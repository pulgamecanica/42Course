import { UserMeContext } from "@/context/UserMeContext";
import { useContext } from "react";

export const useUserMe = () => useContext(UserMeContext);
