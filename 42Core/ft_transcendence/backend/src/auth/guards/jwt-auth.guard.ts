import { ExecutionContext, Injectable } from '@nestjs/common';
import { AuthGuard } from '@nestjs/passport';

@Injectable()
export class JwtAuthGuard extends AuthGuard('jwt') {
  async canActivate(context: ExecutionContext) {
    // Super important, these NEED to be BEFORE canActivate.
    const request = context.switchToHttp().getRequest();
    const isAuth = request.isAuthenticated();

    await super.canActivate(context);

    return isAuth;
  }
}
