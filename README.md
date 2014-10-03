Ipv4 parser
===================

This is a small ipv4 parser written in portable C. It can only parse IPV4 and also it can validate IPV4. And
it is much more faster than inet_pton implementation. It is stably 50-100 times faster than the standard inet_pton
on each platforms and thousands times faster than the boost::asio::from_string. It provides parse and validate 2 
functions. Only IPV4 is supported. The input string must at least have one extra character, so a null terminated 
string is perfectly OK.

#License
public domain

