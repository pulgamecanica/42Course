import { Injectable } from '@nestjs/common';
import { PassportSerializer } from '@nestjs/passport';
import { UserService } from 'src/users/user/user.service';

@Injectable()
export class SessionSerializer extends PassportSerializer {
  constructor(private readonly userService: UserService) {
    super();
  }
  serializeUser(user: any, done: (err: Error, user: any) => void): any {

    done(null, user.username42);
  }
  async deserializeUser(
    payload: any,
    done: (err: Error, user: any) => void,
  ): Promise<any> {

    const user = await this.userService.findUserByUserName42(payload);

    if (!user) {
      done(null, null);
    }
    done(null, user);
  }
}
