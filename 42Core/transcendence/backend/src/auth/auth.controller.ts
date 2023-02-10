import {
  Controller,
  Get,
  Res,
  Req,
  UseGuards,
  Post,
  Body,
  NotAcceptableException,
} from '@nestjs/common';
import { UserService } from 'src/users/user/user.service';
import { AuthService } from './auth.service';
import { FortyTwoAuthGuard } from './guards/42-auth.guard';
import { JwtAuthGuard } from './guards/jwt-auth.guard';

@Controller('auth')
export class AuthController {
  constructor(
    private authService: AuthService,
    private userService: UserService,
  ) {}

  /*******************************************/
  /***            Login / Logout           ***/
  /*******************************************/

  @Get('/login')
  login(@Res() res: any) {
    res.redirect('callback_intra');
  }

  @UseGuards(FortyTwoAuthGuard)
  @Get('/callback_intra')
  async callbackIntra(@Req() req: any, @Res() res: any) {
    const payload = await this.authService.login(req.user);

    if (req.user.isTwoFactorAuthenticationEnabled === true) {
      return res.redirect(
        process.env.FRONTEND_URL + '/2fa?access_token=' + payload.access_token,
      );
    }

    const firstTimeLogin = req.user.firstLogin;

    if (firstTimeLogin) {
      this.userService.update(req.user.id, { ...req.user, firstLogin: false });
      return res.redirect(
        process.env.FRONTEND_URL +
          '/login?access_token=' +
          payload.access_token +
          '&first_login=true',
      );
    } else {
      return res.redirect(
        process.env.FRONTEND_URL +
          '/login?access_token=' +
          payload.access_token,
      );
    }
  }

  @UseGuards(JwtAuthGuard)
  @Post('logout')
  async logout(@Req() req: any, @Res() res: any) {
    const userName = req.user.username;

    return req.logOut(() => {
      res.json({
        user: userName,
        message: 'User has been logged out!',
      });
    });
  }

  /*******************************************/
  /***      Two Factor Authentication      ***/
  /*******************************************/

  @UseGuards(JwtAuthGuard)
  @Post('2fa/generate')
  async generate(@Res() res, @Req() req) {
    const secret =
      await this.authService.generateTwoFactorAuthenticationSecret();
    const otpAuthUrl = await this.authService.generateTwoFactorKeyUri(
      req.user,
      secret,
    );
    await this.userService.setTwoFactorAuthenticationSecret(
      secret,
      req.user.username42,
    );
    return res.json(await this.authService.generateQrCodeDataURL(otpAuthUrl));
  }

  @UseGuards(JwtAuthGuard)
  @Post('2fa/turn-on-off')
  async turnOnOffTwoFactorAuthentication(@Req() req: any, @Body() body: any) {
    const isCodeValid =
      await this.authService.isTwoFactorAuthenticationCodeValid(
        body.twoFactorAuthenticationCode,
        req.user,
      );

    if (!isCodeValid) {
      throw new NotAcceptableException('Wrong authentication code');
    }

    if (req.user.isTwoFactorAuthenticationEnabled === false) {
      await this.userService.setTwoFactorAuthenticationOnAndOff(
        req.user.username42,
      );

      return await this.authenticateTwoFactor(req, body);
    }

    return await this.userService.setTwoFactorAuthenticationOnAndOff(
      req.user.username42,
    );
  }

  @UseGuards(JwtAuthGuard)
  @Post('2fa/authenticate')
  async authenticateTwoFactor(@Req() req: any, @Body() body: any) {
    const isCodeValid =
      await this.authService.isTwoFactorAuthenticationCodeValid(
        body.twoFactorAuthenticationCode,
        req.user,
      );

    if (!isCodeValid) {
      throw new NotAcceptableException('Wrong authentication code');
    }

    return await this.authService.authenticateTwoFactor(req.user);
  }
}
