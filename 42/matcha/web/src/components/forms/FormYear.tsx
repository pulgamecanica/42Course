type FormYearProps = {
  label: string;
  name: string;
  value: number | '';
  onChange: (value: number) => void;
  required?: boolean;
  minYear?: number;
  maxYear?: number;
};

const FormYear = ({
  label,
  name,
  value,
  onChange,
  required = false,
  minYear = 1920,
  maxYear = new Date().getFullYear() - 13,
}: FormYearProps) => {
  const years: number[] = [];
  for (let y = maxYear; y >= minYear; y--) {
    years.push(y);
  }

  return (
    <div className="mb-4">
      <label
        htmlFor={name}
        className="block text-sm font-medium text-gray-700 dark:text-gray-200"
      >
        {label}
      </label>
      <select
        id={name}
        name={name}
        required={required}
        value={value}
        onChange={(e) => onChange(Number(e.target.value))}
        className="mt-1 block w-full px-4 py-2 border border-gray-300 rounded-md bg-white dark:bg-gray-700 text-gray-900 dark:text-white focus:outline-none focus:ring-2 focus:ring-blue-500"
      >
        <option value="">Select year</option>
        {years.map((year) => (
          <option key={year} value={year}>
            {year}
          </option>
        ))}
      </select>
    </div>
  );
};

export default FormYear;
