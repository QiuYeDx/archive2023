# Q: 对于一个二进制流，设计空间复杂度为o(n)的算法，试估算当流进n位二进制时，流中所包含的1的个数，设内存只能存放k位二进制数。
# A: 利用滑动窗口的思想解决

def count_ones(stream, n, k):
    window = stream[:k]
    ones_count = 0
    for i in range(0, k):
        ones_count += int(window[i:i+1])
    total_ones_count = ones_count
    
    for i in range(k, n):
        # 移动窗口，将最左侧的二进制位移除窗口
        tmp = window[1:k]
        
        # 将下一个二进制位添加到窗口中
        tmp += stream[i:i+1]
        
        # 更新当前的1的计数
        # for j in range(0, k):
        #     tmp_count = 0
        #     tmp_count += int(tmp[j:j+1])
        #     total_ones_count += tmp_count
        total_ones_count += int(tmp[k-1:k])

    return total_ones_count

LEN = 16
WINDOW_LEN = 4
print(count_ones(b'1001001001001000', LEN, WINDOW_LEN))