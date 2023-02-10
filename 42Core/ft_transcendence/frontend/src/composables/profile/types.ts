export interface User {
  id?: number
  intraId?: number
  email?: string
  username42?: string
  firstName?: string
  lastName?: string
  nickname?: string
  avatarImg?: string
  bio?: string
  status?: string
  isTwoFactorAuthenticationEnabled?: boolean
  createdAt?: string
  updatedAt?: string
}

export interface ProfileStruct {
  email?: string
  firstName?: string
  lastName?: string
  nickname?: string
  avatarImg?: string
  bio?: string
}