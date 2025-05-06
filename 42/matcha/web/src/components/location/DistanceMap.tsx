import { MapContainer, TileLayer, Marker, Circle } from 'react-leaflet';
import type { LatLngExpression } from 'leaflet';
import 'leaflet/dist/leaflet.css';

type Props = {
  latitude: number;
  longitude: number;
  radiusKm: number | null;
};

export function DistanceMap({ latitude, longitude, radiusKm }: Props) {
  const center: LatLngExpression = [latitude, longitude];
  const radiusMeters = radiusKm ? radiusKm * 1000 : 0;

  return (
    <div className="h-64 mt-4 rounded overflow-hidden border border-gray-300 dark:border-gray-600">
      <MapContainer center={center} zoom={10} scrollWheelZoom={false} className="h-full w-full z-0">
        <TileLayer
          attribution='&copy; OpenStreetMap contributors'
          url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png"
        />
        <Marker position={center} title="" />
        {radiusKm && radiusMeters &&
          <Circle center={center} radius={radiusMeters} pathOptions={{ color: '#007bff', fillOpacity: 0.2 }} />
        }
        </MapContainer>
    </div>
  );
}
