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
export class JwtAuthStrategy extends PassportStrategy(Strategy, 'jwt') {
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
      throw new UnauthorizedException();
    }

    return user;
  }
}
