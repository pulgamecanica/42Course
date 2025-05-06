import {
  Box,
  ToggleButton,
  ToggleButtonGroup,
  Tooltip,
} from "@mui/material";
import LocationOnIcon from "@mui/icons-material/LocationOn";
import TagIcon from "@mui/icons-material/Tag";
import StarIcon from "@mui/icons-material/Star";
import ScoreIcon from "@mui/icons-material/Score";
import CakeIcon from '@mui/icons-material/Cake';
import { SortType } from "@/pages/MatchingPage";

type SortSelectorProps = {
  value: SortType;
  onChange: (value: SortType) => void;
};

export function SortSelector({ value, onChange }: SortSelectorProps) {
  const handleSortChange = (
    _: React.MouseEvent<HTMLElement>,
    newValue: SortType | null
  ) => {
    if (newValue !== null) onChange(newValue);
  };

  return (
    <Box className="flex justify-end">
      <ToggleButtonGroup
        value={value}
        exclusive
        onChange={handleSortChange}
        aria-label="Sort"
        size="small"
        className="bg-gray-100 dark:bg-gray-700 rounded"
      >
        <Tooltip title="Total Score" arrow>
          <ToggleButton value="total_score" className="dark:text-white text-sm">
            <ScoreIcon fontSize="small" />
          </ToggleButton>
        </Tooltip>
        <Tooltip title="Distance" arrow>
          <ToggleButton value="location_score" className="dark:text-white text-sm">
            <LocationOnIcon fontSize="small" />
          </ToggleButton>
        </Tooltip>
        <Tooltip title="Tags Match" arrow>
          <ToggleButton value="tag_score" className="dark:text-white text-sm">
            <TagIcon fontSize="small" />
          </ToggleButton>
        </Tooltip>
        <Tooltip title="Fame Score" arrow>
          <ToggleButton value="fame_score" className="dark:text-white text-sm">
            <StarIcon fontSize="small" />
          </ToggleButton>
        </Tooltip>

        <Tooltip title="Age" arrow>
          <ToggleButton value="age" className="dark:text-white text-sm">
            <CakeIcon fontSize="small" />
          </ToggleButton>
        </Tooltip>
      </ToggleButtonGroup>
    </Box>
  );
}
