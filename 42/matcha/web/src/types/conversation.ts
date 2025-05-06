import { Message } from "@/types/message";
import { PublicUser } from "@/types/user";

export type Conversation = {
  user: PublicUser;
  messages: Message[];
};
