def count_ones(binary_stream, k):
    window = [0] * k  # 初始化大小为k的滑动窗口
    count = 0         # 记录1的个数
    for i in range(len(binary_stream)):
        # 统计当前窗口内1的个数
        ones = sum(window)
        count += ones
        
        # 将新的二进制数加入窗口中
        if binary_stream[i] == '1':
            window.append(1)
            window.pop(0)
        else:
            window.append(0)
            window.pop(0)
    
    # 估算1的个数
    estimated_count = count / (len(binary_stream) / k)
    return estimated_count

# print(count_ones(bytes([17]), 4))
# print(bytes([17]))
print(count_ones(b'1001001001001000', 4))
# print(b'1001001001001000')