int strCompare(const char* str1, const char* str2)
{
	if (!str1 || !str2)
		return -1;

	while (*str1 && *str2)
	{
		if (*str1 < *str2)
		{
			return -1;
		}
		else if (*str2 < *str1)
		{
			return 1;
		}

		str1++;
		str2++;
	}

	if (!*str1 && !*str2)
	{
		return 0;
	}
	else
	{
		return (*str1 ? 1 : -1);
	}
}

void strCopy(const char* source, char* destination)
{
	char* tempPtr = destination;

	while (*source) {
		*tempPtr = *source;
		source++;
		tempPtr++;
	}

	*tempPtr = '\0';
}

unsigned strLength(const char* str)
{
	if (!str)
		return 0;

	unsigned len = 0;
	while (*str)
	{
		len++;
		str++;
	}

	return len;
}