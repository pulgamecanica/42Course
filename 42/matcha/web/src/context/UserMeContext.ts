import { createContext } from 'react';
import { User, PublicUser, UpdateUserProfilePayload } from '@/types/user';
import { Tag } from '@/types/tag';
import { Picture } from '@/types/picture';
import { Location } from '@/types/location';
import { ScheduledDate } from '@/types/scheduledDate';


export type UserMeContextType = {
  user: User | null;
  tags: Tag[];
  pictures: Picture[];
  profilePicture: Picture | null;
  location: Location | null;
  locationHistory: Location[] | null;
  views: PublicUser[];
  viewers: PublicUser[];
  loading: boolean;
  likes: PublicUser[];
  likedBy: PublicUser[];
  matches: PublicUser[];
  connections: PublicUser[]
  profileSetupComplete: boolean;
  scheduledDates: ScheduledDate[];
  setLocationManually: (loc: Location) => void;
  refreshMatches: () => void;
  updateUser: (data: Partial<UpdateUserProfilePayload>) => Promise<void>;
  addTag: (name: string) => Promise<void>;
  removeTag: (name: string) => Promise<void>;
  uploadPicture: (url: string, isProfile?: boolean) => Promise<void>;
  setProfilePicture: (pictureId: number) => Promise<void>;
  deletePicture: (pictureId: number) => Promise<void>;
  addScheduledDate: (date: ScheduledDate) => void;
  reloadScheduledDates: () => void;
  reloadRelationships: () => void;
};

export const UserMeContext = createContext<UserMeContextType>({
  user: null,
  tags: [],
  pictures: [],
  profilePicture: null,
  location: null,
  locationHistory: [],
  views: [],
  viewers: [],
  loading: true,
  likes: [],
  likedBy: [],
  matches: [],
  connections: [],
  profileSetupComplete: false,
  scheduledDates: [],
  setLocationManually: () => {},
  refreshMatches: () => {},
  updateUser: async () => {},
  addTag: async () => {},
  removeTag: async () => {},
  uploadPicture: async () => {},
  setProfilePicture: async () => {},
  deletePicture: async () => {},
  addScheduledDate: () => {},
  reloadScheduledDates: () => {},
  reloadRelationships: () => {},
});
