import { createContext, useContext, ReactNode } from "react";
import { User } from "../types/typings";

interface UserContextType {
  user: User;
  setUser: (user: User) => void;
}

const UserContext = createContext<UserContextType | undefined>(undefined);

interface UserProviderProps {
  user: User;
  setUser: (user: User) => void;
  children: ReactNode;
}

export function UserProvider({ user, setUser, children }: UserProviderProps) {
  const value: UserContextType = { user, setUser };

  return <UserContext.Provider value={value}>{children}</UserContext.Provider>;
}

export function useUser(): UserContextType {
  const context = useContext(UserContext);
  if (!context) {
    throw new Error("useUser must be used within a UserProvider");
  }
  return context;
}
