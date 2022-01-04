from plumbum import local

awk = local['awk']
sed = local['sed']
sort = local['sort']
uniq = local['uniq']
wc = local['wc']
cat = local['cat']

def all_students(file_path):
    awk1 = awk['-F', '\t', '{print $1}']
    cmd = cat[file_path] | awk1
    return cmd()

def number_of_students(file_name):
    awk1 = awk['-F', '\t', '{print $1}']
    cmd = cat[file_path] | awk1 | wc['-l']
    return cmd()

def all_error_codes_with_occurrences(file_path):
    awk1 = awk['-F', '\t', '{print $2}']
    sed1 = sed[r's/[|]/\n/g']
    sed2 = sed[r's/[:].*//g']
    cmd = cat[file_path] | awk1 | sed1 | sed2 | awk['NF'] | sort | uniq['-c']
    return cmd()

def number_error_codes(file_path):
    awk1 = awk['-F', '\t', '{print $2}']
    sed1 = sed[r's/[|]/\n/g']
    sed2 = sed[r's/[:].*//g']
    cmd = cat[file_path] | awk1 | sed1 | sed2 | sort | uniq | awk['NF'] | wc['-l']
    return cmd()


if __name__ == '__main__':
    file_path = 'lab10_grades.txt'
    print('All students list:')
    print(all_students(file_path))
    print(f'Number of students: {number_of_students(file_path)}')
    print('All error codes with number of occurrences:')
    print(all_error_codes_with_occurrences(file_path))
    print(f'Number of unique error codes: {number_error_codes(file_path)}')
