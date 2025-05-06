import { Grid, Skeleton } from '@mui/material';
import { MatchCard } from './MatchCard';
import { SortSelector } from './SortSelector';
import { MatchResult } from '@/types/match';
import { SortType } from '@/pages/MatchingPage';
import { useEffect, useState } from 'react';
import { NoMatches } from './NoMatches';
import { MatchMap } from './MatchMap';
import MapIcon from '@mui/icons-material/Map';
import ListIcon from '@mui/icons-material/ViewList';

type MatchResultsProps = {
  matches: MatchResult[];
  sort: SortType;
  onSortChange: (sort: SortType) => void;
  loading: boolean;
};

export function MatchResults({ matches, sort, onSortChange, loading }: MatchResultsProps) {
  const [view, setView] = useState<'list' | 'map'>(
    () => (localStorage.getItem('matchViewMode') as 'list' | 'map') || 'list'
  );

  const [sortedMatches, setSortedMatches] = useState<MatchResult[]>([]);
  const [isSorting, setIsSorting] = useState(false);

  useEffect(() => {
    setIsSorting(true);
    const timeout = setTimeout(() => {
      const sorted = [...matches].sort((a, b) => {
        if (sort === 'age') {
          const ageA = calcAge(a.user.birth_year);
          const ageB = calcAge(b.user.birth_year);
          return ageA - ageB;
        }
        return parseFloat(b.score[sort] as string) - parseFloat(a.score[sort] as string);
      });
      setSortedMatches(sorted);
      setIsSorting(false);
    }, 300); // simulate short sorting time

    return () => clearTimeout(timeout);
  }, [matches, sort]);

  function handleViewToggle(newView: 'list' | 'map') {
    setView(newView);
    localStorage.setItem('matchViewMode', newView);
  }

  if (loading) return null;
  if (!matches || matches.length === 0) return <NoMatches />;

  return (
    <>
      <div className="flex justify-between items-center mb-4">
        <button
          onClick={() => handleViewToggle(view === 'list' ? 'map' : 'list')}
          className="flex items-center gap-2 px-3 py-2 text-sm font-medium rounded border border-gray-300 dark:border-gray-600 bg-white dark:bg-gray-800 text-gray-800 dark:text-white hover:bg-gray-100 dark:hover:bg-gray-700 transition-colors"
        >
          {view === 'list' ? (
            <>
              <MapIcon fontSize="small" /> Map View
            </>
          ) : (
            <>
              <ListIcon fontSize="small" /> List View
            </>
          )}
        </button>
        {view === 'list' && <SortSelector value={sort} onChange={onSortChange} />}
      </div>

      {view === 'list' ? (
        isSorting ? (
          <Grid container spacing={3}>
            {[...Array(8)].map((_, i) => (
              <Grid item xs={12} sm={6} md={4} lg={3} key={i}>
                <Skeleton variant="rectangular" height={250} />
              </Grid>
            ))}
          </Grid>
        ) : (
          <Grid container spacing={3}>
            {sortedMatches.map((match) => (
              <Grid item xs={12} sm={6} md={4} lg={3} key={match.user.username}>
                <MatchCard match={match} />
              </Grid>
            ))}
          </Grid>
        )
      ) : (
        <MatchMap matches={sortedMatches} />
      )}
    </>
  );
}

function calcAge(birthYear: string) {
  const birth = new Date(`${birthYear}-01-01`);
  const today = new Date();
  let age = today.getFullYear() - birth.getFullYear();
  const m = today.getMonth() - birth.getMonth();
  if (m < 0 || (m === 0 && today.getDate() < birth.getDate())) age--;
  return age;
}
