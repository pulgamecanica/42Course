import { Picture } from '@/types/picture';

type Props = {
  pictures: Picture[];
};

export function PictureGallery({ pictures }: Props) {
  if (!pictures.length) {
    return <p className="mt-4 text-sm text-gray-500 italic">No pictures uploaded yet.</p>;
  }

  return (
    <div className="grid grid-cols-2 md:grid-cols-3 gap-3 mt-4">
      {pictures.map(pic => (
        <img
          key={pic.id}
          src={pic.url}
          alt="User picture"
          className={`rounded-md border shadow-sm ${
            pic.is_profile === 't' ? 'border-green-400' : 'border-gray-300 dark:border-gray-700'
          }`}
        />
      ))}
    </div>
  );
}
