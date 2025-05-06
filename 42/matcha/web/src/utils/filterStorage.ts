import { MatchFilters } from "@/types/match";

const FILTERS_KEY = "match_filters";
const SORT_KEY = "match_sort";

export const saveFilters = (filters: MatchFilters) => {
  localStorage.setItem(FILTERS_KEY, JSON.stringify(filters));
};

export const loadFilters = () => {
  const data = localStorage.getItem(FILTERS_KEY);
  return data ? JSON.parse(data) : null;
};

export const saveSort = (sort: string) => {
  localStorage.setItem(SORT_KEY, sort);
};

export const loadSort = (): string | null => {
  return localStorage.getItem(SORT_KEY);
};
