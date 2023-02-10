import { Injectable } from '@nestjs/common';
import { PassportStrategy } from '@nestjs/passport';
import { Strategy, Profile } from 'passport-42';
import { UserService } from '../../users/user/user.service';
import { CreateUserDto } from 'src/users/user/dto/create-user.dto';

@Injectable()
export class FortyTwoAuthStrategy extends PassportStrategy(Strategy, '42') {
  constructor(private userService: UserService) {
    super({
      clientID: process.env.INTRA_CLIENT_ID,
      clientSecret: process.env.INTRA_CLIENT_SECRET,
      callbackURL: process.env.INTRA_REDIRECT_URI,
    });
  }

  async validate(
    accessToken: string,
    refreshToken: string,
    profile: Profile,
  ): Promise<any> {
    const user = await this.userService.findUserByIntraId(profile.id);

    if (user) {
      return user;
    }

    const newUser: CreateUserDto = {
      intraId: profile.id,
      email: profile._json.email,
      username42: profile._json.login,
      firstName: profile._json.first_name,
      lastName: profile._json.last_name,
      avatarImg: profile._json.image.link,
    };

    const savedUser = this.userService.create(newUser);

    return savedUser;
  }
}
