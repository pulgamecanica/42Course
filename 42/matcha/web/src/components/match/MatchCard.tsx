import {
  Card,
  CardContent,
  CardMedia,
  Typography,
  Box,
  Chip,
  Collapse,
  IconButton,
} from "@mui/material";
import { Circle, ExpandMore, ExpandLess } from "@mui/icons-material";
import { green, red } from "@mui/material/colors";
import { MatchResult } from "@/types/match";
import { useState } from "react";
import { Link } from "react-router-dom";
import { renderGradientBar } from "@/components/FameGradiant";

type MatchCardProps = {
  match: MatchResult;
};

export function MatchCard({ match }: MatchCardProps) {
  const { user, score } = match;
  const profilePic = user.pictures.find((p) => p.is_profile === "t")?.url;
  const age = new Date().getFullYear() - parseInt(user.birth_year, 10);
  const [showDetails, setShowDetails] = useState(false);

  return (
    <Link to={`/profile/${user.username}`} className="no-underline">
      <Card className="bg-white dark:bg-gray-800 dark:text-white shadow-md rounded-lg overflow-hidden hover:shadow-lg hover:ring-2 hover:ring-blue-400 cursor-pointer transition-all duration-200">
        {profilePic ? (
          <CardMedia
            component="img"
            height="200"
            image={profilePic}
            alt={user.username}
            className="object-cover"
          />
        ) : (
          <Box height={200} display="flex" alignItems="center" justifyContent="center" className="bg-gray-200 dark:bg-gray-700">
            <Typography variant="body2" className="text-gray-700 dark:text-white">
              No Image
            </Typography>
          </Box>
        )}

        <CardContent>
          <Typography variant="h6" className="text-gray-900 dark:text-white">
            {user.first_name}
          </Typography>
          <Typography variant="body2" className="text-gray-600 dark:text-gray-300">
            @{user.username}
          </Typography>
          <Typography variant="body2" className="text-gray-600 dark:text-gray-300">
            Age: {age}
          </Typography>

          {/* Total Score */}
          <Box sx={{ mt: 2 }}>
            <Typography variant="body2" className="text-gray-600 dark:text-gray-300">
              Total Score
            </Typography>
            {renderGradientBar("Total Score", score.total)}
          </Box>

          {/* Expand Toggle */}
          <Box className="flex justify-end mt-1">
            <IconButton
              size="small"
              onClick={(e) => {
                e.preventDefault();
                e.stopPropagation();
                setShowDetails(!showDetails);
              }}
              className="dark:text-white"
            >
              {showDetails ? <ExpandLess /> : <ExpandMore />}
            </IconButton>
          </Box>

          {/* Collapsible Detailed Scores */}
          <Collapse in={showDetails}>
            <Box className="space-y-2 mt-1">
              <Box>
                <Typography variant="body2" className="text-gray-600 dark:text-gray-300">
                  Location Score
                </Typography>
                {renderGradientBar("Location Score", score.location_score)}
              </Box>
              <Box>
                <Typography variant="body2" className="text-gray-600 dark:text-gray-300">
                  Tag Score
                </Typography>
                {renderGradientBar("Tag Score", score.tag_score)}
              </Box>
              <Box>
                <Typography variant="body2" className="text-gray-600 dark:text-gray-300">
                  Fame Score
                </Typography>
                {renderGradientBar("Fame Score", parseFloat(score.fame_score))}
              </Box>
            </Box>
          </Collapse>

          {/* Online Status */}
          <Box display="flex" alignItems="center" gap={1} sx={{ mt: 2 }}>
            <Circle
              sx={{
                fontSize: 10,
                color: user.online_status ? green[500] : red[500],
              }}
            />
            <Typography variant="body2" className="text-gray-600 dark:text-gray-300">
              {user.online_status ? "Online" : "Offline"}
            </Typography>
          </Box>

          {/* Tags */}
          {user.tags?.length > 0 && (
            <Box sx={{ mt: 2, display: "flex", flexWrap: "wrap", gap: 1 }}>
              {user.tags.map((tag) => (
                <Chip
                  key={tag.id}
                  label={tag.name}
                  size="small"
                  className="bg-gray-200 dark:bg-gray-700 dark:text-white text-sm px-2"
                />
              ))}
            </Box>
          )}
        </CardContent>
      </Card>
    </Link>
  );
}
