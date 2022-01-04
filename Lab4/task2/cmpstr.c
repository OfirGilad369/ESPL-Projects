int cmpstr(char *s1, char *s2){
	int i=0, result;
	while(s1[i] != '\0' && s2[i] != '\0') {
		result = (int)(s1[i] - s2[i]);
		if(result != 0) {
			return result;
		}
		i = i + 1;
	}
	result = (int)(s1[i] - s2[i]);
	return result;
}
