import { Injectable, UnauthorizedException } from '@nestjs/common';
import { PassportStrategy } from '@nestjs/passport';
import { Strategy, ExtractJwt } from 'passport-jwt';
import { UserService } from 'src/users/user/user.service';

export interface TokenPayload {
  iat: number;
  exp: number;
  username42: string;
  isTwoFactorAuthenticated?: boolean;
  isTwoFactorAuthenticationEnabled: boolean;
}

@Injectable()
export class Jwt2faAuthStrategy extends PassportStrategy(Strategy, 'jwt-2fa') {
  constructor(private readonly userService: UserService) {
    super({
      jwtFromRequest: ExtractJwt.fromAuthHeaderAsBearerToken(),
      secretOrKey: process.env.JWT_SECRET,
    });
  }

  async validate(payload: TokenPayload) {

    const user = await this.userService.findUserByUserName42(
      payload.username42,
    );

    if (!user) {
      return new UnauthorizedException();
    }

    const isPayload2fa = 'isTwoFactorAuthenticationEnabled' in payload;

    if (!user.isTwoFactorAuthenticationEnabled) {
      return user;
    }
    if (!isPayload2fa) {
      return null;
    }
    if (payload.isTwoFactorAuthenticated) {
      return user;
    }
  }
}
