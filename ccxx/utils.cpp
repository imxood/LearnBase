//判断指定长度的字符串是否是全数字
bool allIsNum(const char *str, int length)
{
	for (int i = 0; i < length; i++)
	{
		if (str[i] < 48 || str[i] > 57)
		{
			return false;
		}
	}
	return true;
}