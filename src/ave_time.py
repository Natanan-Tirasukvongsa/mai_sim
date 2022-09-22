# Each_duation = [duration_read_color, duration_read_depth, duration_seg, duration_save_color, duration_save_depth, duration_process]

def ave_duration(All_time_process):
    # initial
    sum_read_color = 0  
    sum_read_depth = 0
    sum_seg = 0
    sum_save_color = 0
    sum_save_depth = 0
    sum_process = 0

    for i in range(len(All_time_process)):
        # summation 
        sum_read_color += All_time_process[i][0]
        sum_read_depth += All_time_process[i][1]
        sum_seg += All_time_process[i][2]
        sum_save_color += All_time_process[i][3]
        sum_save_depth += All_time_process[i][4]
        sum_process += All_time_process[i][5]

    # average = (all summation) / (total number)
    ave_read_color = sum_read_color/len(All_time_process)
    ave_read_depth = sum_read_depth/len(All_time_process)
    ave_seg = sum_seg/len(All_time_process)
    ave_save_color = sum_save_color/len(All_time_process)
    ave_save_depth = sum_save_depth/len(All_time_process)
    ave_process = sum_process/len(All_time_process)
    return ave_read_color, ave_read_depth, ave_seg, ave_save_color, ave_save_depth, ave_process

print(ave_duration(All_time_process))