import { PublicUser } from "./user";

export type MatchFilters = {
  location?: {
    latitude: number;
    longitude: number;
    max_distance_km?: number;
  };
  min_age?: number;
  max_age?: number;
  min_fame?: number;
  tags?: string[];
};


export type MatchScore = {
  location_score: number;
  tag_score: number;
  fame_score: string;
  total: number;
  age: number;
}

export type MatchResult = {
  user: PublicUser;
  score: MatchScore;
}