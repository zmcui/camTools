#! /usr/bin/env python

def printf(_str, _file = None):
	if _file:
		print >>_file,_str
		_file.flush()
	else:
		print _str

def excel_rows(sheet_obj):
	for i in range(sheet_obj.nrows):
		yield sheet_obj.row_values(i)


def blocks(sheet_obj):
	block = []
	base_block = 1
	for row in excel_rows(sheet_obj):
                row = [x.strip() if isinstance(x, unicode) else x for x in row]
		if base_block:
			try: row[0] + ''
			except: continue
			block.append(row)
			if "Offset:" in row[0] and 'Incr_Offset' not in row[0]:
				if block:
					yield block[:-4]
					if any(block[-6]):
						yield block[-6:]
					else:
						yield block[-5:]
					base_block = 0
					block = []
			continue
		if any(row):
			block.append(row)
			try: row[0] + ''
			except: continue
			if 'Offset:' in row[0] and 'Incr_Offset' not in row[0]:
				yield block
				block = []
			continue
		elif len(block) >= 2:
			yield block
			block = []
	else:
		yield block if block else None
