typedef enum {
% if enum.name_lib == "iodef" or len(enum.enum_members) == 0:
        IODEF_${enum.short_type_name.upper()}_ERROR = -1,
% endif
% if len(enum.enum_members) > 0: 
% for member in enum.enum_members:
        ${member.enum_member_desc()}
% endfor
% endif
} ${enum.type_name};
        

