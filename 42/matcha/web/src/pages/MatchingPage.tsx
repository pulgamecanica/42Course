import { useCallback, useEffect, useState } from 'react';
import { discoverMatches } from '@/api/discover';
import { FilterPanel } from '@/components/match/FilterPanel';
import { MatchResults } from '@/components/match/MatchResults';
import { MatchFilters, MatchResult } from '@/types/match';
import {
  loadFilters,
  saveFilters,
  loadSort,
  saveSort,
} from '@/utils/filterStorage';
import { Container, CircularProgress } from '@mui/material';

export type SortType = 'total' | 'location_score' | 'tag_score' | 'fame_score' | 'age';

export function MatchingPage() {
  const [filters, setFilters] = useState<MatchFilters | null>(null);
  const [sort, setSort] = useState<SortType>('total');
  const [results, setResults] = useState<MatchResult[]>([]);
  const [loading, setLoading] = useState(false);

  const fetchMatches = useCallback(
    (filters: MatchFilters) => {
      setLoading(true);
      discoverMatches({ filters })
        .then((result) => {
          setResults(result);
          saveFilters(filters);
        })
        .finally(() => setLoading(false));
    },
    []
  );

  useEffect(() => {
    const savedFilters = loadFilters();
    const savedSort = loadSort();
    if (savedFilters) setFilters(savedFilters);
    if (savedSort) setSort(savedSort as SortType);

    fetchMatches(savedFilters || {});
  }, [fetchMatches]);

  const handleFilterChange = (newFilters: MatchFilters) => {
    saveFilters(newFilters);
    setFilters(newFilters);
    fetchMatches(newFilters);
  };

  return (
    <Container maxWidth="lg" sx={{ py: 4 }}>
      <FilterPanel onSearch={handleFilterChange} initialFilters={filters} />
      {loading ? (
        <CircularProgress sx={{ display: 'block', mx: 'auto', mt: 4 }} />
      ) : (
        <MatchResults
          matches={results}
          sort={sort}
          onSortChange={(s) => {
            setSort(s);
            saveSort(s);
          }}
          loading={loading}
        />
      )}
    </Container>
  );
}
