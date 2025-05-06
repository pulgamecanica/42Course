import { useEffect, useState, useCallback } from 'react';
import axiosInstance from '@api/axios';
import { User, PublicUser, UpdateUserProfilePayload } from '@/types/user';
import { Tag } from '@/types/tag';
import { Picture } from '@/types/picture';
import { Location } from '@/types/location';
import { useAuth } from '@hooks/useAuth';
import toast from 'react-hot-toast';
import { UserMeContext } from '@context/UserMeContext';
import { ScheduledDate } from '@/types/scheduledDate';

export const UserMeProvider = ({ children }: { children: React.ReactNode }) => {
  const [user, setUser] = useState<User | null>(null);
  const [tags, setTags] = useState<Tag[]>([]);
  const [pictures, setPictures] = useState<Picture[]>([]);
  const [profilePicture, setProfilePicture] = useState<Picture | null>(null);
  const [location, setLocation] = useState<Location | null>(null);
  const [locationHistory, setLocationHistory] = useState<Location[] | null>([]);
  const [views, setViews] = useState<PublicUser[]>([]);
  const [viewers, setViewers] = useState<PublicUser[]>([]);
  const [loading, setLoading] = useState(true);
  const [likes, setLikes] = useState<PublicUser[]>([]);
  const [likedBy, setLikedBy] = useState<PublicUser[]>([]);
  const [matches, setMatches] = useState<PublicUser[]>([]);
  const [connections, setConnections] = useState<PublicUser[]>([]);
  const [scheduledDates, setScheduledDates] = useState<ScheduledDate[]>([]);
  const { isAuthenticated } = useAuth();
  
  const setLocationManually = useCallback(async (loc: Location) => {
    if (!loc) return;
    const { city, country } = await reverseGeocodeCity(
      parseFloat(loc.latitude), 
      parseFloat(loc.longitude));

    setLocation({
      ...loc,
      city,
      country,
    });

    try {
      await axiosInstance.post<Location>('/me/location', {
        latitude: loc.latitude,
        longitude: loc.longitude,
        city: city,
        country: country,
      });
      toast.success('Location updated!');
    } catch (err) {
      toast.error(`Failed to update location: ${err}`);
    }
  }, []);

  const fallbackToBrowserLocation = useCallback(() => {
    navigator.geolocation.getCurrentPosition(
      async (pos) => {
        try {
          const response = await axiosInstance.post<Location>('/me/location', {
            latitude: pos.coords.latitude,
            longitude: pos.coords.longitude,
          });
          setLocationManually(response.data);
        } catch (err) {
          toast.error(`Failed to update user location: ${err}`);
        }
      },
      async (err) => {
        toast.error(`Location error: ${err.message}`);
        try {
          const response = await axiosInstance.post<Location>('/me/location', {});
          setLocationManually(response.data);
        } catch (ipErr) {
          toast.error(`Failed to get location by IP: ${ipErr}`);
          setLocation(null);
        }
      }
    );
  }, [setLocationManually]);  


  const reverseGeocodeCity = async (latitude: number, longitude: number): Promise<{ country: string; city: string }>  => {
    try {
      const response = await fetch(
        `https://nominatim.openstreetmap.org/reverse?format=json&lat=${latitude}&lon=${longitude}&zoom=10&addressdetails=1`
      );
      const data = await response.json();
      return {
        country: data.address?.country || 'Moon',
        city: data.address?.city || data.address?.city_district ||	data.address?.suburb || data.address?.county || 'Moonlight'
      };
    } catch (err) {
      toast.error(`Reverse geocoding failed: ${err}`);
      return { country: 'Unknown', city: 'Unknown' };
    }
  }

  useEffect(() => {
    const fetchAll = async () => {
      if (!isAuthenticated) return;

      try {
        const [
          userRes,
          tagsRes,
          picsRes,
          locRes,
          locsRes,
          viewsRes,
          viewersRes,
          likesRes,
          likedByRes,
          matchesRes,
          connectionsRes,
          scheduledDatesRes,
        ] = await Promise.all([
          axiosInstance.get<User>('/me') as unknown as User,
          axiosInstance.get('/me/tags') as unknown as Tag[],
          axiosInstance.get('/me/pictures') as unknown as Picture[],
          axiosInstance.get('/me/location') as unknown as Location,
          axiosInstance.get('/me/location/history') as unknown as Location[],
          axiosInstance.get('/me/views') as unknown as PublicUser[],
          axiosInstance.get('/me/visits') as unknown as PublicUser[],
          axiosInstance.get('/me/likes') as unknown as PublicUser[],
          axiosInstance.get('/me/liked_by') as unknown as PublicUser[],
          axiosInstance.get('/me/matches') as unknown as PublicUser[],
          axiosInstance.get('/me/connections') as unknown as PublicUser[],
          axiosInstance.get('/me/dates') as unknown as ScheduledDate[],
        ]);

        setUser(userRes);
        setTags(tagsRes);
        setPictures(picsRes.filter((pic: Picture) => pic.is_profile !== 't') || null);
        setProfilePicture(picsRes.find((pic: Picture) => pic.is_profile === 't') || null);
        setLocationHistory(locsRes);
        setLocation(locRes);
        setViews(viewsRes);
        setViewers(viewersRes);
        setLikes(likesRes);
        setLikedBy(likedByRes);
        setMatches(matchesRes);
        setConnections(connectionsRes);
        setScheduledDates(scheduledDatesRes)
        if (!userRes?.longitude || !userRes?.latitude || !locRes || !locRes.latitude || !locRes.longitude) {
          fallbackToBrowserLocation();
        }
      } catch (err) {
        toast.error(`Failed to fetch user profile: ${err}`);
      } finally {
        setLoading(false);
      }
    }

    fetchAll();
  }, [isAuthenticated, fallbackToBrowserLocation]);

  const updateUser = async (data: Partial<UpdateUserProfilePayload>) => {
    try {
      const res = await axiosInstance.patch<User>('/me', data);
      setUser(res as unknown as User);
    } catch (err) {
      toast.error('Failed to update user profile.');
      throw err;
    }
  };

  const addTag = async (name: string) => {
    try {
      const newTag = await axiosInstance.post<Tag>('/me/tags', { name });
      setTags(prev => [...prev, newTag as unknown as Tag]);
    } catch (err) {
      toast.error('Failed to add tag');
      throw err;
    }
  };

  const removeTag = async (name: string) => {
    try {
      await axiosInstance.delete('/me/tags', {
        data: { name },
      });
      setTags((prev) => prev.filter((tag) => tag.name !== name));
    } catch (err) {
      toast.error('Failed to remove tag');
      throw err;
    }
  };

  const uploadPicture = async (url: string, isProfile = false) => {
    try {
      const res = await axiosInstance.post<Picture>('/me/pictures', { url, is_profile: isProfile });
      const newPic = res as unknown as Picture;
      if (isProfile) {
        setProfilePicture(newPic);
        setPictures(prev => prev.filter(p => p.id !== newPic.id));
      } else {
        setPictures(prev => [...prev, newPic]);
      }
    } catch (err) {
      toast.error('Failed to upload picture');
      throw err;
    }
  };

  const setProfilePictureById = async (pictureId: number) => {
    try {
      const res = await axiosInstance.patch<Picture>(`/me/pictures/${pictureId}`, {
        is_profile: true,
      });

      const newProfile = res as unknown as Picture;

      if (profilePicture) {
        setPictures(prev => [...prev, { ...profilePicture, is_profile: 'f' }]);
      }

      setPictures(prev => prev.filter(p => p.id !== newProfile.id));

      setProfilePicture({ ...newProfile, is_profile: 't' });
    } catch (err) {
      toast.error('Failed to set profile picture');
      throw err;
    }
  };

  const deletePicture = async (pictureId: number) => {
    try {
      await axiosInstance.delete(`/me/pictures/${pictureId}`);
      setPictures(prev => prev.filter(p => p.id !== pictureId));
      if (profilePicture?.id === pictureId) {
        setProfilePicture(null);
      }
    } catch (err) {
      toast.error('Failed to delete picture');
      throw err;
    }
  };

  const refreshMatches = async () => {
    try {
      setMatches(await axiosInstance.get('/me/matches') as unknown as PublicUser[]);
    } catch (err) {
      toast.error('Failed to delete picture');
      throw err;
    }
  };

  const addScheduledDate = (date : ScheduledDate) => {
    setScheduledDates((prev) => {
      return [...prev, date];
    })
  };

  const reloadScheduledDates = async () => {
    try {
      setScheduledDates(await axiosInstance.get('/me/dates') as unknown as ScheduledDate[]);
    } catch (err) {
      toast.error('Failed to retrieve your dates');
      throw err;
    }
  }

  const reloadRelationships = async () => {
    try {
      setLikedBy(await axiosInstance.get('/me/liked_by') as unknown as PublicUser[])
      setLikes(await axiosInstance.get('/me/likes') as unknown as PublicUser[])
      setMatches(await axiosInstance.get('/me/matches') as unknown as PublicUser[]);
      setViews(await axiosInstance.get('/me/views') as unknown as PublicUser[]);
      setViewers(await axiosInstance.get('/me/visits') as unknown as PublicUser[]);
    } catch {
      toast.error('Failed to retrieve your relationship');
    }
  }

  return (
      <UserMeContext.Provider
        value={{
            user,
            tags,
            pictures,
            profilePicture,
            location,
            locationHistory,
            views,
            viewers,
            loading,
            likes,
            likedBy,
            setLocationManually,
            refreshMatches,
            updateUser,
            addTag,
            removeTag,
            uploadPicture,
            setProfilePicture: setProfilePictureById,
            deletePicture,
            addScheduledDate,
            profileSetupComplete: !!(user?.gender && user?.sexual_preferences),
            matches,
            connections,
            scheduledDates,
            reloadScheduledDates,
            reloadRelationships,
        }}
      >
        {children}
      </UserMeContext.Provider>
    );
}
