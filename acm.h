#pragma once
#ifndef acm
#define acm

#include <iostream>
#include <vector>
#include <stack>
#include <string>
using namespace std;

struct ListNode {
	int val;
	ListNode *next;
	ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
public:
	char a[1000][1000];

	bool isMatch(string s, string p) {
		cout << s << "," << p << endl;
		getchar();
		int i=0, j=0;

		while (i < s.size() && j < p.size()) {
			//cout << i << "," <<j << endl;
			if (s[i] == p[j] &&(( j + 1 < p.size() && p[j + 1] != '*')||j+1==p.size())) {
				i++; j++;
				continue;
			}
			else if (s[i] == p[j] && j + 1 < p.size() && p[j + 1] == '*') {
				int tmp;
				if (isMatch(s.substr(i, s.size() -i), p.substr(j + 2, p.size() - j - 2)))
					return true;
				for (tmp = i;tmp < s.size();tmp++) {
					if (s[tmp] == s[i]) {
						if (isMatch(s.substr(tmp+1,s.size()-tmp-1),p.substr(j+2,p.size()-j-2)))
							return true;
					}
					else {
						break;
					}
				}
				if (s[tmp] != s[i]) {
					return false;
				}
			}
			else if (p[j] == '.'&& ((j + 1 < p.size() && p[j + 1] != '*') || j + 1 == p.size())){
				i++; j++;
				continue;
			}
			else if (p[j] == '.'&&j + 1 < p.size() && p[j + 1] == '*') {
				int tmp;
				if (isMatch(s.substr(i, s.size() - i), p.substr(j + 2, p.size() - j - 2)))
					return true;
				for (tmp = i;tmp < s.size();tmp++) {
					if (isMatch(s.substr(tmp + 1, s.size() - tmp-1), p.substr(j + 2, p.size() - j - 2)))
						return true;
				}
				return false;
			}
			else if (p[j] != s[i] && ((j + 1 < p.size() && p[j + 1] != '*') || j + 1 == p.size()))
				return false;
			else if (p[j] != s[i] && j + 1 < p.size() && p[j + 1] == '*') {
				if (isMatch(s.substr(i,s.size() - i), p.substr(j + 2, p.size() - j - 2)))
					return true;
				else return false;
			}
		}
		if (i == s.size()){
			bool final = true;
			bool state = true;
			for (;j < p.size();j++) {
				if (p[j]!='*') {
					if (state == false)
					{
						final = false;
						return false;
					}
					state = false;
				}
				else {
					state = true;
				}
			}
			return state;
		}
		else if (j == p.size()) {
			bool final = true;
			bool state = true;
			for (;i < s.size();i++) {
				if (s[i] != '*') {
					if (state == false)
					{
						final = false;
						return false;
					}
					state = false;
				}
				else {
					state = true;
				}
			}
			return state;
		}
		else return false;
	}

	bool isPalindrome(int x) {
		bool state = true;
		x = x > 0 ? x : -x;
		int i,j;
		vector<int> list;
		while (x != 0) {
			int tmp = x % 10;
			x /= 10;
			list.push_back(tmp);
		}
		for (int i = 0;i < list.size() / 2;i++) {
			if (list[i]!=list[list.size()-1-i]) {
				state = false;
				break;
			}
		}
		return state;

	}

	

	int getReverse(int x) {
		int result = 0;
		while (x) {
			int tmp = x % 10;
			x /= 10;
			result *= 10;
			result += tmp;
		}
		return result<0?0:result;
	}
	int reverse(int x) {
		if (x > 0) {
			return getReverse(x);
		}
		else {
			return -getReverse(-x);
		}
	}


	string convert(string s, int numRows) {
		int i, j;
		//cout << 2 * s.size() / (numRows + 1) + 1 << endl;
		if (numRows == 1) {
			return s;
		}
		for (j = 0;j < numRows;j++) {
			//cout << (numRows - 1)*((s.size() + 1) / (2 * numRows - 2)) + 1 << endl;
			//cout << (numRows - 1)*((s.size()) / (2 * numRows - 2)) + 1 << endl;
			for (i = 0;i <= 1000;i++) {
				a[j][i] = '\0';
			}
		}
		int pos = 0;
		while (pos < s.size()) {
			if (pos % (2*numRows-2)>=0&& pos % (2 * numRows - 2)< numRows) {
				//cout <<"11111:"<< pos % (2 * numRows - 2) << "," << (numRows - 1)*(pos / (2 * numRows - 2)) <<":"<<s[pos] << endl;
				a[pos % (2 * numRows - 2)][(numRows - 1)*(pos / (2 * numRows - 2))] = s[pos];
			}else {
				//cout << "22222:" << numRows - ((pos) % (2 * numRows - 2) - numRows)-2 << "," << (numRows - 1)*((pos) / (2 * numRows - 2)) + (pos) % (2 * numRows - 2)- numRows + 1 << ":" << s[pos] << endl;
				a[numRows - ((pos) % (2 * numRows - 2) - numRows)-2][(numRows - 1)*((pos) / (2 * numRows - 2)) + (pos) % (2 * numRows - 2) - numRows + 1] = s[pos];
			}
			pos++;
		}
		string result = "";
		for (j = 0;j < numRows;j++) {
			for (i = 0;i <= 1000;i++) {
				//cout <<j<<","<<i<< a[j][i] << endl;
				if (a[j][i] != '\0') {
					result += a[j][i];
				}
			}
		}
		return result;
	}

	
	string longestPalindrome(string s) {
		int i, j,k;
		
		for (i = 0;i < s.size();i++) {
			a[i][i] = 1;
		}
		//cout << "here1" << endl;
		for (i = 0;i < s.size() - 1;i++) {
			if (s[i] == s[i + 1]) {
				a[i][i + 1] = 2;
			}
		}
		//cout << "here2" << endl;
		for (i = s.size()-3;i >=0;i--) {
			for (j = i + 2;j < s.size();j++) {
				if (s[i] == s[j]&& a[i + 1][j - 1]!=0) {
					a[i][j] = a[i + 1][j - 1] + 2;
				}else {
					a[i][j] = 0;
				}
			}
		}
		int max = 0;
		int pos1=0;
		for (i = 0;i < s.size();i++) {
			for (j = i;j < s.size();j++) {
				if (a[i][j] > max) {
					max = a[i][j];
					pos1 = i;
				}
			}
		}
		return s.substr(pos1,max);
	}

	double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
		double result;
		vector<int> tmp;
		int i=0, j=0;
		while (i < nums1.size() && j < nums2.size())
		{
			if (nums1[i] > nums2[j]) {
				tmp.push_back(nums2[j]);
				j++;
			}else {
				tmp.push_back(nums1[i]);
				i++;
			}
		}
		while (i < nums1.size())
		{	
			tmp.push_back(nums1[i]);
			i++;
		}
		while (j < nums2.size())
		{
			tmp.push_back(nums2[j]);
			j++;
		}
		if (tmp.size() % 2 == 0) {
			return (tmp[tmp.size() / 2]+ tmp[tmp.size() / 2-1])/2;
		}
		else {
			return tmp[tmp.size() / 2];
		}
	}

	int lengthOfLongestSubstring(string s) {
		int max=0;
		int i,j,k;
		bool ch[256];
		for (i = 0;i < s.size();i++) {
			for (j = 0;j < 256;j++)
				ch[j] = false;
			for (j = i;j < s.size();j ++ ) {
				if (ch[s[j]]) {
					break;
				}
				ch[s[j]] = true;
			}
			if (j - i > max) {
				max = j - i;
			}
		}
		return max;
	}

	ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
		ListNode* result = new ListNode(-1);;
		ListNode* ListNodeTmp = result;
		int tmp=0;
		while (l1&&l2) {
			int t = l1->val + l2->val + tmp;
			tmp = t / 10;
			t %= 10;
			ListNodeTmp->val = t;
			ListNodeTmp->next = new ListNode(-1);
			ListNodeTmp = ListNodeTmp->next;
			l1 = l1->next;
			l2 = l2->next;
		}
		while (l1) {
			int t = l1->val + tmp;
			tmp = t / 10;
			t %= 10;
			ListNodeTmp->val = t;
			ListNodeTmp->next = new ListNode(-1);
			ListNodeTmp = ListNodeTmp->next;
			l1 = l1->next;
		}
		while (l2) {
			int t =  l2->val + tmp;
			tmp = t / 10;
			t %= 10;
			ListNodeTmp->val = t;
			ListNodeTmp->next = new ListNode(-1);
			ListNodeTmp = ListNodeTmp->next;
			l2 = l2->next;
		}

		if (tmp != 0) {
			ListNodeTmp->val = tmp;
			ListNodeTmp->next = new ListNode(-1);
			ListNodeTmp = ListNodeTmp->next;
		}

		ListNodeTmp->next = NULL;
		return result;

	}

	vector<int> twoSum(vector<int>& nums, int target) {
		vector<int> result;
		int i, j;
		for (i = 0;i < nums.size();i++) {
			for (j = i + 1;j < nums.size();j++) {
				if (nums[i] + nums[j] == target) {
					result.push_back(i);
					result.push_back(j);
					return result;
				}
			}
		}
		return result;
	}

	int getNumber(string data) {
		int result=0;
		bool fu = false;
		int i;
		if (data[0] == '-') {
			fu = true;
			for (i = 1;i < data.length();i++) {
				result *= 10;
				result += (data[i] - '0');
			}
			result = -result;
		}
		else {
			for (i = 0;i < data.length();i++) {
				result *= 10;
				result += (data[i] - '0');
			}
		}
		return result;
	}
	bool isNumber(string data) {
		if ((data[0] >= '0'&&data[0] <= '9')||(data[0]=='-'&&data.size()!=1)) {
			return true;
		}
		else return false;
	}
	int evalRPN(vector<string>& tokens) {
		int result = 0;
		stack<int> mystack;
		int i = 0;
		for (i = 0;i < tokens.size();i++) {
			//cout << tokens[i]<<","<<isNumber(tokens[i]) << endl;
			if (isNumber(tokens[i])) {
				mystack.push(getNumber(tokens[i]));
			}
			else {
				int number1 = mystack.top();
				mystack.pop();
				int number2 = mystack.top();
				mystack.pop();
				int tmp = 0;
				//cout << tokens[0][0] << endl;
				switch (tokens[i][0]) {
				case '+':
					tmp = number2 + number1;
					break;
				case '-':
					tmp = number2 - number1;
					break;
				case '*':
					tmp = number2 * number1;
					break;
				case '/':
					tmp = number2 / number1;
					break;
				}
				mystack.push(tmp);
			}
		}
		result = mystack.top();
		return result;
	}
};

#endif