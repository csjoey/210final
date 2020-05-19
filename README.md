# 210final
Joseph Loporto 210 Final for playing a stacker game between two pi's interfaced together by ssh tunnel

On the server side, the user executes this command:

ssh -R "same#":localhost:"same#" <eecis user>@go.eecis.udel.edu -fN

On the client side, the user executes the command:

ssh -L "same#":localhost:"same# <other eecis user>@go.eecis.udel.edu -fN

This connects port 8080 on the client to port 8080 on the server.



To play the game connect the two pi's and on the client side mak the first move

the game ends when a block is placed using the joystick that does not line up with the previous ones in a straight line, or the top is reached.
