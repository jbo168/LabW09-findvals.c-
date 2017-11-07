
int approxEqual(float el, float refval, float tolerance)
{
  return (refval-tolerance <= el &&
	  el <= refval+tolerance);
}


int inRange(char c, char l, char r)
{
  return (l <= c && c <= r);
}

char flipRange(char c, char l, char r)
{
    return r - (c - l);
}


char flipChar(char c)
{
  if (inRange(c, '0', '9'))
    return flipRange(c, '0','9');

  if (inRange(c, 'a', 'z'))
    return flipRange(c, 'a','z');

  if (inRange(c, 'A', 'Z'))
    return flipRange(c, 'A','Z');

  return c;
}
