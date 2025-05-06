type StatItemProps = {
    label: string;
    value: number;
    color: string;
    onClick: () => void;
  };
  
  export function StatItem({ label, value, color, onClick }: StatItemProps) {
    return (
      <div onClick={onClick} className="flex flex-col items-center cursor-pointer">
        <span className={`text-lg font-semibold ${color}`}>{value}</span>
        <span className="text-xs">{label}</span>
      </div>
    );
  }
  