#encoding=utf-8

def insert_sort(nums):
    for i in range(1, len(nums)):
        j = i - 1
        tmpnum = nums[i]
        while j >= 0 and tmpnum < nums[j]:
            nums[j+1] = nums[j]
            j -= 1
        nums[j+1] = tmpnum
    return nums    

nums = [44, 12, 59, 36, 62, 43, 94, 7, 35, 52, 85] 
print(insert_sort(nums))
