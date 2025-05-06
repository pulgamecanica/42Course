import { useState } from 'react';
import { useUserMe } from '@/hooks/useUserMe';
import { Tag } from '@/types/tag';
import toast from 'react-hot-toast';

export const TagEditor = () => {
  const { tags, addTag, removeTag } = useUserMe();
  const [newTag, setNewTag] = useState('');
  const [submitting, setSubmitting] = useState(false);

  const handleAddTag = async (e: React.FormEvent) => {
    e.preventDefault();
    const trimmed = newTag.trim().toLowerCase();
    if (!trimmed) return;

    setSubmitting(true);
    try {
      await addTag(trimmed);
      setNewTag('');
      toast.success(`Added tag "${trimmed}"`);
    } catch {
      toast.error('Could not add tag');
    } finally {
      setSubmitting(false);
    }
  };

  const handleRemove = async (tag: Tag) => {
    try {
      await removeTag(tag.name);
      toast.success(`Removed tag "${tag.name}"`);
    } catch {
      toast.error('Could not remove tag');
    }
  };

  return (
    <div className="mb-6">
      <h3 className="text-lg font-semibold mb-2">Tags</h3>
      <form onSubmit={handleAddTag} className="flex gap-2">
        <input
          type="text"
          value={newTag}
          onChange={(e) => setNewTag(e.target.value)}
          placeholder="Add a tag..."
          className="flex-1 px-2 py-1 rounded border border-gray-300 dark:border-gray-600 bg-white dark:bg-gray-900"
          disabled={submitting}
        />
        <button
          type="submit"
          className="px-3 py-1 bg-blue-600 text-white rounded hover:bg-blue-700 disabled:opacity-50"
          disabled={submitting}
        >
          Add
        </button>
      </form>

      <div className="mt-4 flex flex-wrap gap-2">
        {tags.map((tag) => (
          <span
            key={tag.id}
            className="bg-gray-200 dark:bg-gray-700 text-sm px-3 py-1 rounded-full flex items-center"
          >
            {tag.name}
            <button
              onClick={() => handleRemove(tag)}
              className="ml-2 text-red-600 dark:text-red-400 font-bold"
              title="Remove tag"
            >
              ×
            </button>
          </span>
        ))}
      </div>
    </div>
  );
};
