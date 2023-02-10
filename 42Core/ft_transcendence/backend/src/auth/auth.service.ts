import { Injectable } from '@nestjs/common';
import { JwtService } from '@nestjs/jwt';
import { User } from 'src/users/user/entities/user.entity';
import { authenticator } from 'otplib';
import { toDataURL } from 'qrcode';

@Injectable()
export class AuthService {
  constructor(
    private jwtService: JwtService,
  ) {}

  /*******************************************/
  /***            Login Logic              ***/
  /*******************************************/

  async login(user: User) {
    const payload = {
      username42: user.username42,
    };

    return {
      username42: payload.username42,
      access_token: this.jwtService.sign(payload),
    };
  }

  /*******************************************/
  /***             Verify Logic            ***/
  /*******************************************/

  async verify(token: string): Promise<boolean> {
    try {
      const validated = this.jwtService.verify(token, {
        secret: process.env.JWT_SECRET,
      });
      return true;
    } catch (error) {
      return false;
    }
    return false;
  }

  /*******************************************/
  /***   Two Factor Authentication Logic   ***/
  /*******************************************/

  async generateTwoFactorAuthenticationSecret() {
    return authenticator.generateSecret();
  }

  async generateTwoFactorKeyUri(user: User, secret: string) {
    return authenticator.keyuri(user.username42, 'ft_transcendence', secret);
  }

  async generateQrCodeDataURL(otpAuthUrl: string) {
    return toDataURL(otpAuthUrl);
  }

  async isTwoFactorAuthenticationCodeValid(
    twoFactorAuthenticationCode: string,
    user: User,
  ) {
    return authenticator.verify({
      token: twoFactorAuthenticationCode,
      secret: user.twoFactorAuthenticationSecret,
    });
  }

  async authenticateTwoFactor(user: User) {
    const payload = {
      username42: user.username42,
      isTwoFactorAuthenticationEnabled: user.isTwoFactorAuthenticationEnabled,
      isTwoFactorAuthenticated: true,
    };

    return {
      username42: payload.username42,
      access_token: this.jwtService.sign(payload),
    };
  }
}
