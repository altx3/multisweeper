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
