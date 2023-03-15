#! /usr/bin/env python
import re,copy


class Handler:
	def callback(self, prefix, name, *args):
		method = getattr(self, prefix+name, None)
		if callable(method): return method(*args)
	def do(self,name,*args):
		self.callback('do_',name,*args)

class RegisterRenderer(Handler):
	"""docstring for RegisterRenderer"""
	def do_capbase(self,block,register,regfield):
		block = [row[0].upper() for row in block]
		for _string in block:
			match =  re.search('BASE[ ]?:(.*?)0X([a-fA-F0-9\_]+)',_string)
			if match:
				module_name = match.group(1).strip()
				module_name = module_name.split()[0]
				module_name = module_name if 'BASE' in module_name else module_name + '_BASE'
				base_addr = '0x' + match.group(2).strip().replace('_','')
				register.addBase(module_name, base_addr)
				# print module_name,base_addr

	def do_capregister(self,block,register,regfield):
		try:
			block = [ value for value in block if value[0].strip()]
			if len(block) == 4:
				short_desc = ''
				reg_name = block[0][0].strip()
				detail = block[1][0].strip()
				number = block[2][0].replace('Number:','').strip()
				incr_offset = 0
			elif len(block) == 5:
				if 'Incr_Offset:' in block[3][0]:
					short_desc = ''
					reg_name = block[0][0].strip()
					detail = block[1][0].strip()
					number = block[2][0].replace('Number:','').strip()
					incr_offset = block[3][0].replace('Incr_Offset:','')
					incr_offset = eval(incr_offset.strip())
				else:
					short_desc = block[0][0].strip()
					reg_name = block[1][0].strip()
					detail = block[2][0].strip()
					number = block[3][0].replace('Number:','').strip()
					incr_offset = 0
			elif len(block) == 6:
				short_desc = block[0][0].strip()
				reg_name = block[1][0].strip()
				detail = block[2][0].strip()
				number = block[3][0].replace('Number:','').strip()
				incr_offset = block[4][0].replace('Incr_Offset:','')
				incr_offset = eval(incr_offset.strip())
			else:
				raise Exception,"File type Error"
			off_set_str = block[-1][0].strip().upper()
			off_set = off_set_str.split('T:')#Offset:0x0 OR Offset:0x0-0x4 OR Offset:0x0 - 0x4
			off_set = off_set[1].strip()
			off_set_l = [ eval(off_set) + i*incr_offset for i in range(eval(number))]
			off_set_l = [str(hex(l)) for l in off_set_l]
			register.fillInfo(reg_name, short_desc, detail,number)
			for offset in off_set_l:
				register.addOffset(offset)
		except Exception,e:
			pass
			print "Error:",e
			print block

	def do_capregfield(self,block,register,regfield):
		keys = [_str.strip() for _str in block[0]]
		for row in block[1:]:
			field_d = {key.strip():str(value).strip() for key,value in zip(keys,row)}
			try:
				regfield.update(field_d)
			except Exception,e:
				pass
				# print e
				# print field_d
				# raw_input()
			register.addRegField(copy.deepcopy(regfield))


