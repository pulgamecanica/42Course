import { ReactNode } from 'react';
import { Link } from 'react-router-dom';

type DashboardCardProps = {
  title: string;
  icon: ReactNode;
  to?: string;
  description: string;
  onClick?: () => void;
};

export function DashboardCard({ title, icon, to, description, onClick }: DashboardCardProps) {
  const CardContent = (
    <div className="bg-white text-gray-900 mx-1 p-6 rounded-xl shadow-md flex flex-col items-center space-y-4 dark:bg-gray-700 dark:text-white hover:scale-105 transition-all duration-300">
      <div className="text-3xl">{icon}</div>
      <div className="text-xl font-semibold">{title}</div>
      <p className="text-sm text-gray-600 dark:text-gray-300 text-center">{description}</p>
    </div>
  );

  return onClick ? (
    <button onClick={onClick} className="w-full text-left">{CardContent}</button>
  ) : (
    <Link to={to!}>{CardContent}</Link>
  );
}