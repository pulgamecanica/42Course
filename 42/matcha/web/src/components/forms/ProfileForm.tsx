import React, { useEffect, useState } from 'react';
import FormInput from '@components/forms/FormInput';
import FormSelect from '@components/forms/FormSelect';
import FormYear from '@components/forms/FormYear';
import { useUserMe } from '@/hooks/useUserMe';
import { Gender, SexualPreference } from '@/types/user';
import { TagEditor } from '@components/profile/TagEditor';
import { PictureManager } from '@components/profile/PictureManager';

type ProfileFormProps = {
  onSubmit: (data: {
    username: string;
    first_name: string;
    last_name: string;
    gender: 'male' | 'female' | 'other';
    sexual_preferences: 'male' | 'female' | 'non_binary' | 'everyone';
    biography: string;
    birth_year: number;
  }) => void;
  buttonText?: string;
};

export const ProfileForm = ({ onSubmit, buttonText = 'Save' }: ProfileFormProps) => {
  const { user } = useUserMe();
  const [username, setUsername] = useState('');
  const [firstName, setFirstName] = useState('');
  const [lastName, setLastName] = useState('');
  const [gender, setGender] = useState<Gender | ''>('');
  const [sexualPreferences, setSexualPreferences] = useState<SexualPreference | ''>('');
  const [biography, setBiography] = useState('');
  const [birthYear, setBirthYear] = useState<number | ''>('');

  useEffect(() => {
    if (user) {
      setUsername(user.username);
      setFirstName(user.first_name);
      setLastName(user.last_name);
      setGender(user.gender);
      setSexualPreferences(user.sexual_preferences);
      setBiography(user.biography || '');
      setBirthYear(user.birth_year || '');
    }
  }, [user])

  const handleSubmit = (e: React.FormEvent) => {
    e.preventDefault();
    onSubmit({
      username,
      first_name: firstName,
      last_name: lastName,
      gender: gender as 'male' | 'female' | 'other',
      sexual_preferences: sexualPreferences as 'male' | 'female' | 'non_binary' | 'everyone',
      biography,
      birth_year: Number(birthYear),
    });
  };

  return (
    <>
      <TagEditor />
      <PictureManager />
      <form onSubmit={handleSubmit} className="mt-6">
        <FormInput
          label="Username"
          name="username"
          value={username}
          onChange={(e) => setUsername(e.target.value)}
          required
        />
        <FormInput
          label="First Name"
          name="first_name"
          value={firstName}
          onChange={(e) => setFirstName(e.target.value)}
        />
        <FormInput
          label="Last Name"
          name="last_name"
          value={lastName}
          onChange={(e) => setLastName(e.target.value)}
        />
        <FormSelect
          label="Gender"
          name="gender"
          value={gender}
          onChange={(e) => setGender(e.target.value as Gender)}
          options={['male', 'female', 'other']}
          required
        />
        <FormSelect
          label="Sexual Preferences"
          name="sexual_preferences"
          value={sexualPreferences}
          onChange={(e) => setSexualPreferences(e.target.value as SexualPreference)}
          options={['male', 'female', 'non_binary', 'everyone']}
          required
        />
        <FormInput
          label="Biography"
          name="biography"
          value={biography}
          onChange={(e) => setBiography(e.target.value)}
          placeholder="Tell us a little about yourself"
        />
        <FormYear
          label="Birth Year"
          name="birth_year"
          value={birthYear}
          onChange={setBirthYear}
          required
        />
        <button
          type="submit"
          className="w-full py-2 px-4 bg-blue-600 hover:bg-blue-700 text-white rounded-md mt-4"
        >
          {buttonText}
        </button>
      </form>
    </>
  );
};
