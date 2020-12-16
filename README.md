# 210final
Joseph Loporto 210 Final for playing a stacker game between two sensehat pi's interfaced together by ssh tunnel

On the server side, the user executes this command:

ssh -R "same#":localhost:"same#" <user>@server -fN

On the client side, the user executes the command:

ssh -L "same#":localhost:"same# <user>@server -fN

This connects port 8080 on the client to port 8080 on the server.



-To play the game connect the two pi's and on the client side make the first move, I reccomend forwarding ports of the same number to each other.

-The game ends when a block is placed using the joystick that does not line up with the previous ones in a straight line, or the top is reached.

-Pkill scrollnum reccomended if using sensehat default configuration.

-Because of the nature of the execution this game can be played on a single pi with a client and server running.
