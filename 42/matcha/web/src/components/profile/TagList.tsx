import { Tag } from '@/types/tag';

type Props = {
  tags: Tag[];
};

export function TagList({ tags }: Props) {
  const tagColors = ['pink', 'blue', 'green', 'purple', 'yellow'];

  function getTagColor(tag: string) {
    const index = tag.charCodeAt(0) % tagColors.length;
    return tagColors[index];
  }
  
  if (!tags.length) return null;

  return (
    <div className="flex flex-wrap gap-2 mt-4">
      {tags.map(tag => (
        <span
          key={tag.id}
          className={`border px-3 py-2 rounded-full bg-${getTagColor(tag.name)}-200 text-purple-800 text-xs font-medium dark:bg-${getTagColor(tag.name)}-900 dark:text-purple-100`}
        >
          #{tag.name}
        </span>
      ))}
    </div>
  );
}
