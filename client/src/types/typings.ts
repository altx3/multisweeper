export interface Lobby {
  host_id: string;
  lobby_id: string;
  players: string[];
}

export interface CreateLobbyResponse {
  lobby_id: string;
  player_id: string;
}
export interface GetLobbiesResponse {
  lobbies: Lobby[];
}

export interface User {
  player_id: string;
  username: string;
  lobby_id: string;
}
