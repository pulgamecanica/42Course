import { Location } from '@/types/location';

type Props = {
  location: Location | null;
};

export function LocationCard({ location }: Props) {
  if (!location) return null;

  const { city, country, latitude, longitude } = location;

  const locationText =
    city && country
      ? `${city}, ${country}`
      : latitude && longitude
        ? `${parseInt(latitude).toFixed(4)}, ${parseInt(longitude).toFixed(4)}`
        : 'Unknown location';

  return (
    <div className="mt-4 text-sm text-gray-500">
      📍 Last seen in <strong>{locationText}</strong>
    </div>
  );
}
