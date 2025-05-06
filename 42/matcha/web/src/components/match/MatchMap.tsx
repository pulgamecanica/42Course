/* eslint-disable @typescript-eslint/no-explicit-any */

import { MapContainer, TileLayer, Marker, Popup } from 'react-leaflet';
import { MatchResult } from '@/types/match';
import L from 'leaflet';
import { Link } from 'react-router-dom';
import 'leaflet/dist/leaflet.css';

type MatchMapProps = {
  matches: MatchResult[];
};

export function MatchMap({ matches }: MatchMapProps) {
  const validMatches = matches.filter(
    (m) => m.user.latitude !== null && m.user.longitude !== null
  );

  const center: [number, number] =
    validMatches.length > 0
      ? [validMatches[0].user.latitude!, validMatches[0].user.longitude!]
      : [48.0, 2.0];

  delete (L.Icon.Default.prototype as any)._getIconUrl;
  L.Icon.Default.mergeOptions({
    iconRetinaUrl: '/marker-icon-2x.png',
    iconUrl: '/marker-icon.png',
    shadowUrl: '/marker-shadow.png',
  });

  return (
    <div className="h-[70vh] w-full rounded overflow-hidden border border-gray-300 dark:border-gray-600 z-10">
      <MapContainer center={center} zoom={5} scrollWheelZoom className="h-full w-full">
        <TileLayer
          attribution='&copy; OpenStreetMap contributors'
          url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png"
        />
        {validMatches.map((match) => {
          const user = match.user;
          const profilePic =
            user.pictures.find((p) => p.id === user.profile_picture_id)?.url ||
            '/default-avatar.png';

          const customIcon = L.icon({
            iconUrl: profilePic,
            iconSize: [40, 40],
            iconAnchor: [20, 40],
            className: 'rounded-full border border-white shadow-md',
          });

          return (
            <Marker
              key={user.id}
              position={[user.latitude!, user.longitude!]}
              icon={customIcon}
            >
              <Popup>
                <div className="text-center w-44">
                  <Link
                    to={`/profile/${user.username}`}
                    className="no-underline text-blue-600 hover:underline"
                  >
                    <img
                      src={profilePic}
                      alt={`${user.username}'s avatar`}
                      className="w-16 h-16 rounded-full mx-auto mb-2 object-cover border border-gray-300"
                    />
                    <p className="text-sm font-semibold text-gray-800">
                      {user.first_name} {user.last_name}
                    </p>
                    <p className="text-xs text-gray-500">@{user.username}</p>
                    <p className="text-xs mt-1">Click to view profile</p>
                  </Link>
                </div>
              </Popup>
            </Marker>
          );
        })}
      </MapContainer>
    </div>
  );
}
