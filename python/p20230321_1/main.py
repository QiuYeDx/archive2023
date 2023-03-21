# Q: 设计亚线性算法检查两个字符串是否相同.

# A: 要设计一个亚线性算法来检查两个字符串是否相同，可以使用Bloom Filter。

# Bloom Filter是一种概率数据结构，它可以有效地检查一个元素是否属于一个集合。
# 它的主要思想是使用多个哈希函数将元素映射到一个位数组中，并将其对应的位设置为1。
# 当查询一个元素时，将其哈希到位数组中，并检查对应的位是否都为1。
# 如果有任何一个位为0，则该元素不属于集合中。如果所有位都为1，则该元素可能属于集合中，但可能是假阳性。在这种情况下，需要进行更详细的检查以确定该元素是否真正存在于集合中。

# 对于检查两个字符串是否相同，可以将它们分别插入两个Bloom Filter中，并检查它们的位数组是否完全相同。
# 如果两个位数组不完全相同，则可以确定这两个字符串不相同。如果两个位数组完全相同，则需要进一步检查这两个字符串是否真正相同。
# 这可以通过常规的字符串比较算法来完成，因为布隆过滤器已经排除了大部分不相同的情况，所以字符串比较的成本将大大降低。
# 因此，该算法的时间复杂度为O(n)，其中n是字符串的长度，这是一个亚线性算法。

import hashlib
 
class BloomFilter:
    def __init__(self, n, k):
        self.bits = [False] * n
        self.k = k
        
    def add(self, value):
        for i in range(self.k):
            hash_value = int(hashlib.sha256(str(value + str(i)).encode()).hexdigest(), 16)
            index = hash_value % len(self.bits)
            self.bits[index] = True
            
    def contains(self, value):
        for i in range(self.k):
            hash_value = int(hashlib.sha256(str(value + str(i)).encode()).hexdigest(), 16)
            index = hash_value % len(self.bits)
            if not self.bits[index]:
                return False
        return True
 
def are_strings_equal(s1, s2):
    if len(s1) != len(s2):
        return False
    bloom1 = BloomFilter(10000, 5)  # 创建两个Bloom Filter实例
    bloom2 = BloomFilter(10000, 5)
    for i in range(len(s1)):
        bloom1.add(s1[i])  # 将字符串1中的字符添加到bloom1中
        bloom2.add(s2[i])  # 将字符串2中的字符添加到bloom2中
    if bloom1.bits != bloom2.bits:  # 检查两个位数组是否相同
        return False
    return s1 == s2  # 如果位数组相同，则进一步检查字符串是否相同
 
# 示例用法
s1 = "hello"
s2 = "world"
s3 = "hello"
print(are_strings_equal(s1, s2))  # False
print(are_strings_equal(s1, s3))  # True
