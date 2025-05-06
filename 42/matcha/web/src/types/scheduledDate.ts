export type ScheduledDate = {
    id: string;
    connection_id: string;
    initiator_id: string;
    location: string;
    scheduled_at: string;
    note: string | null;
    created_at: string;
    partner_username: string;
};
  
export type ScheduledDateResponse = {
  message: string;
  data: ScheduledDate[];
};

export type SetDatePayload = {
  username: string; 
  scheduled_at: string;
  location?: string;
  note?: string;
};