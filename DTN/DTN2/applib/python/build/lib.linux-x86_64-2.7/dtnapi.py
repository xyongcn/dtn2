# This file was automatically generated by SWIG (http://www.swig.org).
# Version 1.3.35
#
# Don't modify this file, modify the SWIG interface instead.
# This file is compatible with both classic and new-style classes.

import _dtnapi
import new
new_instancemethod = new.instancemethod
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'PySwigObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static) or hasattr(self,name):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError,name

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

DTN_MAX_ENDPOINT_ID = _dtnapi.DTN_MAX_ENDPOINT_ID
DTN_MAX_AUTHDATA = _dtnapi.DTN_MAX_AUTHDATA
DTN_MAX_REGION_LEN = _dtnapi.DTN_MAX_REGION_LEN
DTN_MAX_BUNDLE_MEM = _dtnapi.DTN_MAX_BUNDLE_MEM
DTN_MAX_BLOCK_LEN = _dtnapi.DTN_MAX_BLOCK_LEN
DTN_MAX_BLOCKS = _dtnapi.DTN_MAX_BLOCKS
class dtn_endpoint_id_t:
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, dtn_endpoint_id_t, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, dtn_endpoint_id_t, name)
    __repr__ = _swig_repr
    __swig_setmethods__["uri"] = _dtnapi.dtn_endpoint_id_t_uri_set
    __swig_getmethods__["uri"] = _dtnapi.dtn_endpoint_id_t_uri_get
    def __init__(self, *args): 
        this = apply(_dtnapi.new_dtn_endpoint_id_t, args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _dtnapi.delete_dtn_endpoint_id_t
    __del__ = lambda self : None;
dtn_endpoint_id_t_swigregister = _dtnapi.dtn_endpoint_id_t_swigregister
dtn_endpoint_id_t_swigregister(dtn_endpoint_id_t)

class dtn_timestamp_t:
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, dtn_timestamp_t, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, dtn_timestamp_t, name)
    __repr__ = _swig_repr
    __swig_setmethods__["secs"] = _dtnapi.dtn_timestamp_t_secs_set
    __swig_getmethods__["secs"] = _dtnapi.dtn_timestamp_t_secs_get
    __swig_setmethods__["seqno"] = _dtnapi.dtn_timestamp_t_seqno_set
    __swig_getmethods__["seqno"] = _dtnapi.dtn_timestamp_t_seqno_get
    def __init__(self, *args): 
        this = apply(_dtnapi.new_dtn_timestamp_t, args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _dtnapi.delete_dtn_timestamp_t
    __del__ = lambda self : None;
dtn_timestamp_t_swigregister = _dtnapi.dtn_timestamp_t_swigregister
dtn_timestamp_t_swigregister(dtn_timestamp_t)

class dtn_service_tag_t:
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, dtn_service_tag_t, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, dtn_service_tag_t, name)
    __repr__ = _swig_repr
    __swig_setmethods__["tag"] = _dtnapi.dtn_service_tag_t_tag_set
    __swig_getmethods__["tag"] = _dtnapi.dtn_service_tag_t_tag_get
    def __init__(self, *args): 
        this = apply(_dtnapi.new_dtn_service_tag_t, args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _dtnapi.delete_dtn_service_tag_t
    __del__ = lambda self : None;
dtn_service_tag_t_swigregister = _dtnapi.dtn_service_tag_t_swigregister
dtn_service_tag_t_swigregister(dtn_service_tag_t)

DTN_REGID_NONE = _dtnapi.DTN_REGID_NONE
DTN_REG_DROP = _dtnapi.DTN_REG_DROP
DTN_REG_DEFER = _dtnapi.DTN_REG_DEFER
DTN_REG_EXEC = _dtnapi.DTN_REG_EXEC
DTN_SESSION_CUSTODY = _dtnapi.DTN_SESSION_CUSTODY
DTN_SESSION_PUBLISH = _dtnapi.DTN_SESSION_PUBLISH
DTN_SESSION_SUBSCRIBE = _dtnapi.DTN_SESSION_SUBSCRIBE
class dtn_reg_info_t:
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, dtn_reg_info_t, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, dtn_reg_info_t, name)
    __repr__ = _swig_repr
    __swig_setmethods__["endpoint"] = _dtnapi.dtn_reg_info_t_endpoint_set
    __swig_getmethods__["endpoint"] = _dtnapi.dtn_reg_info_t_endpoint_get
    __swig_setmethods__["regid"] = _dtnapi.dtn_reg_info_t_regid_set
    __swig_getmethods__["regid"] = _dtnapi.dtn_reg_info_t_regid_get
    __swig_setmethods__["flags"] = _dtnapi.dtn_reg_info_t_flags_set
    __swig_getmethods__["flags"] = _dtnapi.dtn_reg_info_t_flags_get
    __swig_setmethods__["expiration"] = _dtnapi.dtn_reg_info_t_expiration_set
    __swig_getmethods__["expiration"] = _dtnapi.dtn_reg_info_t_expiration_get
    __swig_setmethods__["init_passive"] = _dtnapi.dtn_reg_info_t_init_passive_set
    __swig_getmethods__["init_passive"] = _dtnapi.dtn_reg_info_t_init_passive_get
    __swig_getmethods__["script"] = _dtnapi.dtn_reg_info_t_script_get
    def __init__(self, *args): 
        this = apply(_dtnapi.new_dtn_reg_info_t, args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _dtnapi.delete_dtn_reg_info_t
    __del__ = lambda self : None;
dtn_reg_info_t_swigregister = _dtnapi.dtn_reg_info_t_swigregister
dtn_reg_info_t_swigregister(dtn_reg_info_t)

class dtn_reg_info_t_script:
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, dtn_reg_info_t_script, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, dtn_reg_info_t_script, name)
    __repr__ = _swig_repr
    __swig_setmethods__["script_len"] = _dtnapi.dtn_reg_info_t_script_script_len_set
    __swig_getmethods__["script_len"] = _dtnapi.dtn_reg_info_t_script_script_len_get
    __swig_setmethods__["script_val"] = _dtnapi.dtn_reg_info_t_script_script_val_set
    __swig_getmethods__["script_val"] = _dtnapi.dtn_reg_info_t_script_script_val_get
    def __init__(self, *args): 
        this = apply(_dtnapi.new_dtn_reg_info_t_script, args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _dtnapi.delete_dtn_reg_info_t_script
    __del__ = lambda self : None;
dtn_reg_info_t_script_swigregister = _dtnapi.dtn_reg_info_t_script_swigregister
dtn_reg_info_t_script_swigregister(dtn_reg_info_t_script)

COS_BULK = _dtnapi.COS_BULK
COS_NORMAL = _dtnapi.COS_NORMAL
COS_EXPEDITED = _dtnapi.COS_EXPEDITED
COS_RESERVED = _dtnapi.COS_RESERVED
DOPTS_NONE = _dtnapi.DOPTS_NONE
DOPTS_CUSTODY = _dtnapi.DOPTS_CUSTODY
DOPTS_DELIVERY_RCPT = _dtnapi.DOPTS_DELIVERY_RCPT
DOPTS_RECEIVE_RCPT = _dtnapi.DOPTS_RECEIVE_RCPT
DOPTS_FORWARD_RCPT = _dtnapi.DOPTS_FORWARD_RCPT
DOPTS_CUSTODY_RCPT = _dtnapi.DOPTS_CUSTODY_RCPT
DOPTS_DELETE_RCPT = _dtnapi.DOPTS_DELETE_RCPT
DOPTS_SINGLETON_DEST = _dtnapi.DOPTS_SINGLETON_DEST
DOPTS_MULTINODE_DEST = _dtnapi.DOPTS_MULTINODE_DEST
DOPTS_DO_NOT_FRAGMENT = _dtnapi.DOPTS_DO_NOT_FRAGMENT
BLOCK_FLAG_NONE = _dtnapi.BLOCK_FLAG_NONE
BLOCK_FLAG_REPLICATE = _dtnapi.BLOCK_FLAG_REPLICATE
BLOCK_FLAG_REPORT = _dtnapi.BLOCK_FLAG_REPORT
BLOCK_FLAG_DELETE_BUNDLE = _dtnapi.BLOCK_FLAG_DELETE_BUNDLE
BLOCK_FLAG_LAST = _dtnapi.BLOCK_FLAG_LAST
BLOCK_FLAG_DISCARD_BLOCK = _dtnapi.BLOCK_FLAG_DISCARD_BLOCK
BLOCK_FLAG_UNPROCESSED = _dtnapi.BLOCK_FLAG_UNPROCESSED
class dtn_extension_block_t:
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, dtn_extension_block_t, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, dtn_extension_block_t, name)
    __repr__ = _swig_repr
    __swig_setmethods__["type"] = _dtnapi.dtn_extension_block_t_type_set
    __swig_getmethods__["type"] = _dtnapi.dtn_extension_block_t_type_get
    __swig_setmethods__["flags"] = _dtnapi.dtn_extension_block_t_flags_set
    __swig_getmethods__["flags"] = _dtnapi.dtn_extension_block_t_flags_get
    __swig_getmethods__["data"] = _dtnapi.dtn_extension_block_t_data_get
    def __init__(self, *args): 
        this = apply(_dtnapi.new_dtn_extension_block_t, args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _dtnapi.delete_dtn_extension_block_t
    __del__ = lambda self : None;
dtn_extension_block_t_swigregister = _dtnapi.dtn_extension_block_t_swigregister
dtn_extension_block_t_swigregister(dtn_extension_block_t)

class dtn_extension_block_t_data:
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, dtn_extension_block_t_data, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, dtn_extension_block_t_data, name)
    __repr__ = _swig_repr
    __swig_setmethods__["data_len"] = _dtnapi.dtn_extension_block_t_data_data_len_set
    __swig_getmethods__["data_len"] = _dtnapi.dtn_extension_block_t_data_data_len_get
    __swig_setmethods__["data_val"] = _dtnapi.dtn_extension_block_t_data_data_val_set
    __swig_getmethods__["data_val"] = _dtnapi.dtn_extension_block_t_data_data_val_get
    def __init__(self, *args): 
        this = apply(_dtnapi.new_dtn_extension_block_t_data, args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _dtnapi.delete_dtn_extension_block_t_data
    __del__ = lambda self : None;
dtn_extension_block_t_data_swigregister = _dtnapi.dtn_extension_block_t_data_swigregister
dtn_extension_block_t_data_swigregister(dtn_extension_block_t_data)

class dtn_sequence_id_t:
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, dtn_sequence_id_t, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, dtn_sequence_id_t, name)
    __repr__ = _swig_repr
    __swig_getmethods__["data"] = _dtnapi.dtn_sequence_id_t_data_get
    def __init__(self, *args): 
        this = apply(_dtnapi.new_dtn_sequence_id_t, args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _dtnapi.delete_dtn_sequence_id_t
    __del__ = lambda self : None;
dtn_sequence_id_t_swigregister = _dtnapi.dtn_sequence_id_t_swigregister
dtn_sequence_id_t_swigregister(dtn_sequence_id_t)

class dtn_sequence_id_t_data:
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, dtn_sequence_id_t_data, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, dtn_sequence_id_t_data, name)
    __repr__ = _swig_repr
    __swig_setmethods__["data_len"] = _dtnapi.dtn_sequence_id_t_data_data_len_set
    __swig_getmethods__["data_len"] = _dtnapi.dtn_sequence_id_t_data_data_len_get
    __swig_setmethods__["data_val"] = _dtnapi.dtn_sequence_id_t_data_data_val_set
    __swig_getmethods__["data_val"] = _dtnapi.dtn_sequence_id_t_data_data_val_get
    def __init__(self, *args): 
        this = apply(_dtnapi.new_dtn_sequence_id_t_data, args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _dtnapi.delete_dtn_sequence_id_t_data
    __del__ = lambda self : None;
dtn_sequence_id_t_data_swigregister = _dtnapi.dtn_sequence_id_t_data_swigregister
dtn_sequence_id_t_data_swigregister(dtn_sequence_id_t_data)

class dtn_bundle_spec_t:
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, dtn_bundle_spec_t, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, dtn_bundle_spec_t, name)
    __repr__ = _swig_repr
    __swig_setmethods__["source"] = _dtnapi.dtn_bundle_spec_t_source_set
    __swig_getmethods__["source"] = _dtnapi.dtn_bundle_spec_t_source_get
    __swig_setmethods__["dest"] = _dtnapi.dtn_bundle_spec_t_dest_set
    __swig_getmethods__["dest"] = _dtnapi.dtn_bundle_spec_t_dest_get
    __swig_setmethods__["replyto"] = _dtnapi.dtn_bundle_spec_t_replyto_set
    __swig_getmethods__["replyto"] = _dtnapi.dtn_bundle_spec_t_replyto_get
    __swig_setmethods__["priority"] = _dtnapi.dtn_bundle_spec_t_priority_set
    __swig_getmethods__["priority"] = _dtnapi.dtn_bundle_spec_t_priority_get
    __swig_setmethods__["dopts"] = _dtnapi.dtn_bundle_spec_t_dopts_set
    __swig_getmethods__["dopts"] = _dtnapi.dtn_bundle_spec_t_dopts_get
    __swig_setmethods__["expiration"] = _dtnapi.dtn_bundle_spec_t_expiration_set
    __swig_getmethods__["expiration"] = _dtnapi.dtn_bundle_spec_t_expiration_get
    __swig_setmethods__["creation_ts"] = _dtnapi.dtn_bundle_spec_t_creation_ts_set
    __swig_getmethods__["creation_ts"] = _dtnapi.dtn_bundle_spec_t_creation_ts_get
    __swig_setmethods__["delivery_regid"] = _dtnapi.dtn_bundle_spec_t_delivery_regid_set
    __swig_getmethods__["delivery_regid"] = _dtnapi.dtn_bundle_spec_t_delivery_regid_get
    __swig_setmethods__["sequence_id"] = _dtnapi.dtn_bundle_spec_t_sequence_id_set
    __swig_getmethods__["sequence_id"] = _dtnapi.dtn_bundle_spec_t_sequence_id_get
    __swig_setmethods__["obsoletes_id"] = _dtnapi.dtn_bundle_spec_t_obsoletes_id_set
    __swig_getmethods__["obsoletes_id"] = _dtnapi.dtn_bundle_spec_t_obsoletes_id_get
    __swig_getmethods__["metadata"] = _dtnapi.dtn_bundle_spec_t_metadata_get
    __swig_getmethods__["blocks"] = _dtnapi.dtn_bundle_spec_t_blocks_get
    def __init__(self, *args): 
        this = apply(_dtnapi.new_dtn_bundle_spec_t, args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _dtnapi.delete_dtn_bundle_spec_t
    __del__ = lambda self : None;
dtn_bundle_spec_t_swigregister = _dtnapi.dtn_bundle_spec_t_swigregister
dtn_bundle_spec_t_swigregister(dtn_bundle_spec_t)

class dtn_bundle_spec_t_metadata:
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, dtn_bundle_spec_t_metadata, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, dtn_bundle_spec_t_metadata, name)
    __repr__ = _swig_repr
    __swig_setmethods__["metadata_len"] = _dtnapi.dtn_bundle_spec_t_metadata_metadata_len_set
    __swig_getmethods__["metadata_len"] = _dtnapi.dtn_bundle_spec_t_metadata_metadata_len_get
    __swig_setmethods__["metadata_val"] = _dtnapi.dtn_bundle_spec_t_metadata_metadata_val_set
    __swig_getmethods__["metadata_val"] = _dtnapi.dtn_bundle_spec_t_metadata_metadata_val_get
    def __init__(self, *args): 
        this = apply(_dtnapi.new_dtn_bundle_spec_t_metadata, args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _dtnapi.delete_dtn_bundle_spec_t_metadata
    __del__ = lambda self : None;
dtn_bundle_spec_t_metadata_swigregister = _dtnapi.dtn_bundle_spec_t_metadata_swigregister
dtn_bundle_spec_t_metadata_swigregister(dtn_bundle_spec_t_metadata)

class dtn_bundle_spec_t_blocks:
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, dtn_bundle_spec_t_blocks, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, dtn_bundle_spec_t_blocks, name)
    __repr__ = _swig_repr
    __swig_setmethods__["blocks_len"] = _dtnapi.dtn_bundle_spec_t_blocks_blocks_len_set
    __swig_getmethods__["blocks_len"] = _dtnapi.dtn_bundle_spec_t_blocks_blocks_len_get
    __swig_setmethods__["blocks_val"] = _dtnapi.dtn_bundle_spec_t_blocks_blocks_val_set
    __swig_getmethods__["blocks_val"] = _dtnapi.dtn_bundle_spec_t_blocks_blocks_val_get
    def __init__(self, *args): 
        this = apply(_dtnapi.new_dtn_bundle_spec_t_blocks, args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _dtnapi.delete_dtn_bundle_spec_t_blocks
    __del__ = lambda self : None;
dtn_bundle_spec_t_blocks_swigregister = _dtnapi.dtn_bundle_spec_t_blocks_swigregister
dtn_bundle_spec_t_blocks_swigregister(dtn_bundle_spec_t_blocks)

class dtn_bundle_id_t:
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, dtn_bundle_id_t, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, dtn_bundle_id_t, name)
    __repr__ = _swig_repr
    __swig_setmethods__["source"] = _dtnapi.dtn_bundle_id_t_source_set
    __swig_getmethods__["source"] = _dtnapi.dtn_bundle_id_t_source_get
    __swig_setmethods__["creation_ts"] = _dtnapi.dtn_bundle_id_t_creation_ts_set
    __swig_getmethods__["creation_ts"] = _dtnapi.dtn_bundle_id_t_creation_ts_get
    __swig_setmethods__["frag_offset"] = _dtnapi.dtn_bundle_id_t_frag_offset_set
    __swig_getmethods__["frag_offset"] = _dtnapi.dtn_bundle_id_t_frag_offset_get
    __swig_setmethods__["orig_length"] = _dtnapi.dtn_bundle_id_t_orig_length_set
    __swig_getmethods__["orig_length"] = _dtnapi.dtn_bundle_id_t_orig_length_get
    def __init__(self, *args): 
        this = apply(_dtnapi.new_dtn_bundle_id_t, args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _dtnapi.delete_dtn_bundle_id_t
    __del__ = lambda self : None;
dtn_bundle_id_t_swigregister = _dtnapi.dtn_bundle_id_t_swigregister
dtn_bundle_id_t_swigregister(dtn_bundle_id_t)

REASON_NO_ADDTL_INFO = _dtnapi.REASON_NO_ADDTL_INFO
REASON_LIFETIME_EXPIRED = _dtnapi.REASON_LIFETIME_EXPIRED
REASON_FORWARDED_UNIDIR_LINK = _dtnapi.REASON_FORWARDED_UNIDIR_LINK
REASON_TRANSMISSION_CANCELLED = _dtnapi.REASON_TRANSMISSION_CANCELLED
REASON_DEPLETED_STORAGE = _dtnapi.REASON_DEPLETED_STORAGE
REASON_ENDPOINT_ID_UNINTELLIGIBLE = _dtnapi.REASON_ENDPOINT_ID_UNINTELLIGIBLE
REASON_NO_ROUTE_TO_DEST = _dtnapi.REASON_NO_ROUTE_TO_DEST
REASON_NO_TIMELY_CONTACT = _dtnapi.REASON_NO_TIMELY_CONTACT
REASON_BLOCK_UNINTELLIGIBLE = _dtnapi.REASON_BLOCK_UNINTELLIGIBLE
STATUS_RECEIVED = _dtnapi.STATUS_RECEIVED
STATUS_CUSTODY_ACCEPTED = _dtnapi.STATUS_CUSTODY_ACCEPTED
STATUS_FORWARDED = _dtnapi.STATUS_FORWARDED
STATUS_DELIVERED = _dtnapi.STATUS_DELIVERED
STATUS_DELETED = _dtnapi.STATUS_DELETED
STATUS_ACKED_BY_APP = _dtnapi.STATUS_ACKED_BY_APP
class dtn_bundle_status_report_t:
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, dtn_bundle_status_report_t, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, dtn_bundle_status_report_t, name)
    __repr__ = _swig_repr
    __swig_setmethods__["bundle_id"] = _dtnapi.dtn_bundle_status_report_t_bundle_id_set
    __swig_getmethods__["bundle_id"] = _dtnapi.dtn_bundle_status_report_t_bundle_id_get
    __swig_setmethods__["reason"] = _dtnapi.dtn_bundle_status_report_t_reason_set
    __swig_getmethods__["reason"] = _dtnapi.dtn_bundle_status_report_t_reason_get
    __swig_setmethods__["flags"] = _dtnapi.dtn_bundle_status_report_t_flags_set
    __swig_getmethods__["flags"] = _dtnapi.dtn_bundle_status_report_t_flags_get
    __swig_setmethods__["receipt_ts"] = _dtnapi.dtn_bundle_status_report_t_receipt_ts_set
    __swig_getmethods__["receipt_ts"] = _dtnapi.dtn_bundle_status_report_t_receipt_ts_get
    __swig_setmethods__["custody_ts"] = _dtnapi.dtn_bundle_status_report_t_custody_ts_set
    __swig_getmethods__["custody_ts"] = _dtnapi.dtn_bundle_status_report_t_custody_ts_get
    __swig_setmethods__["forwarding_ts"] = _dtnapi.dtn_bundle_status_report_t_forwarding_ts_set
    __swig_getmethods__["forwarding_ts"] = _dtnapi.dtn_bundle_status_report_t_forwarding_ts_get
    __swig_setmethods__["delivery_ts"] = _dtnapi.dtn_bundle_status_report_t_delivery_ts_set
    __swig_getmethods__["delivery_ts"] = _dtnapi.dtn_bundle_status_report_t_delivery_ts_get
    __swig_setmethods__["deletion_ts"] = _dtnapi.dtn_bundle_status_report_t_deletion_ts_set
    __swig_getmethods__["deletion_ts"] = _dtnapi.dtn_bundle_status_report_t_deletion_ts_get
    __swig_setmethods__["ack_by_app_ts"] = _dtnapi.dtn_bundle_status_report_t_ack_by_app_ts_set
    __swig_getmethods__["ack_by_app_ts"] = _dtnapi.dtn_bundle_status_report_t_ack_by_app_ts_get
    def __init__(self, *args): 
        this = apply(_dtnapi.new_dtn_bundle_status_report_t, args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _dtnapi.delete_dtn_bundle_status_report_t
    __del__ = lambda self : None;
dtn_bundle_status_report_t_swigregister = _dtnapi.dtn_bundle_status_report_t_swigregister
dtn_bundle_status_report_t_swigregister(dtn_bundle_status_report_t)

DTN_PAYLOAD_FILE = _dtnapi.DTN_PAYLOAD_FILE
DTN_PAYLOAD_MEM = _dtnapi.DTN_PAYLOAD_MEM
DTN_PAYLOAD_TEMP_FILE = _dtnapi.DTN_PAYLOAD_TEMP_FILE
class dtn_bundle_payload_t:
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, dtn_bundle_payload_t, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, dtn_bundle_payload_t, name)
    __repr__ = _swig_repr
    __swig_setmethods__["location"] = _dtnapi.dtn_bundle_payload_t_location_set
    __swig_getmethods__["location"] = _dtnapi.dtn_bundle_payload_t_location_get
    __swig_setmethods__["status_report"] = _dtnapi.dtn_bundle_payload_t_status_report_set
    __swig_getmethods__["status_report"] = _dtnapi.dtn_bundle_payload_t_status_report_get
    __swig_getmethods__["buf"] = _dtnapi.dtn_bundle_payload_t_buf_get
    __swig_getmethods__["filename"] = _dtnapi.dtn_bundle_payload_t_filename_get
    def __init__(self, *args): 
        this = apply(_dtnapi.new_dtn_bundle_payload_t, args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _dtnapi.delete_dtn_bundle_payload_t
    __del__ = lambda self : None;
dtn_bundle_payload_t_swigregister = _dtnapi.dtn_bundle_payload_t_swigregister
dtn_bundle_payload_t_swigregister(dtn_bundle_payload_t)

class dtn_bundle_payload_t_buf:
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, dtn_bundle_payload_t_buf, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, dtn_bundle_payload_t_buf, name)
    __repr__ = _swig_repr
    __swig_setmethods__["buf_len"] = _dtnapi.dtn_bundle_payload_t_buf_buf_len_set
    __swig_getmethods__["buf_len"] = _dtnapi.dtn_bundle_payload_t_buf_buf_len_get
    __swig_setmethods__["buf_val"] = _dtnapi.dtn_bundle_payload_t_buf_buf_val_set
    __swig_getmethods__["buf_val"] = _dtnapi.dtn_bundle_payload_t_buf_buf_val_get
    def __init__(self, *args): 
        this = apply(_dtnapi.new_dtn_bundle_payload_t_buf, args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _dtnapi.delete_dtn_bundle_payload_t_buf
    __del__ = lambda self : None;
dtn_bundle_payload_t_buf_swigregister = _dtnapi.dtn_bundle_payload_t_buf_swigregister
dtn_bundle_payload_t_buf_swigregister(dtn_bundle_payload_t_buf)

class dtn_bundle_payload_t_filename:
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, dtn_bundle_payload_t_filename, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, dtn_bundle_payload_t_filename, name)
    __repr__ = _swig_repr
    __swig_setmethods__["filename_len"] = _dtnapi.dtn_bundle_payload_t_filename_filename_len_set
    __swig_getmethods__["filename_len"] = _dtnapi.dtn_bundle_payload_t_filename_filename_len_get
    __swig_setmethods__["filename_val"] = _dtnapi.dtn_bundle_payload_t_filename_filename_val_set
    __swig_getmethods__["filename_val"] = _dtnapi.dtn_bundle_payload_t_filename_filename_val_get
    def __init__(self, *args): 
        this = apply(_dtnapi.new_dtn_bundle_payload_t_filename, args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _dtnapi.delete_dtn_bundle_payload_t_filename
    __del__ = lambda self : None;
dtn_bundle_payload_t_filename_swigregister = _dtnapi.dtn_bundle_payload_t_filename_swigregister
dtn_bundle_payload_t_filename_swigregister(dtn_bundle_payload_t_filename)

xdr_dtn_endpoint_id_t = _dtnapi.xdr_dtn_endpoint_id_t
xdr_dtn_reg_id_t = _dtnapi.xdr_dtn_reg_id_t
xdr_dtn_timeval_t = _dtnapi.xdr_dtn_timeval_t
xdr_dtn_timestamp_t = _dtnapi.xdr_dtn_timestamp_t
xdr_dtn_service_tag_t = _dtnapi.xdr_dtn_service_tag_t
xdr_dtn_reg_flags_t = _dtnapi.xdr_dtn_reg_flags_t
xdr_dtn_reg_info_t = _dtnapi.xdr_dtn_reg_info_t
xdr_dtn_bundle_priority_t = _dtnapi.xdr_dtn_bundle_priority_t
xdr_dtn_bundle_delivery_opts_t = _dtnapi.xdr_dtn_bundle_delivery_opts_t
xdr_dtn_extension_block_flags_t = _dtnapi.xdr_dtn_extension_block_flags_t
xdr_dtn_extension_block_t = _dtnapi.xdr_dtn_extension_block_t
xdr_dtn_sequence_id_t = _dtnapi.xdr_dtn_sequence_id_t
xdr_dtn_bundle_spec_t = _dtnapi.xdr_dtn_bundle_spec_t
xdr_dtn_bundle_id_t = _dtnapi.xdr_dtn_bundle_id_t
xdr_dtn_status_report_reason_t = _dtnapi.xdr_dtn_status_report_reason_t
xdr_dtn_status_report_flags_t = _dtnapi.xdr_dtn_status_report_flags_t
xdr_dtn_bundle_status_report_t = _dtnapi.xdr_dtn_bundle_status_report_t
xdr_dtn_bundle_payload_location_t = _dtnapi.xdr_dtn_bundle_payload_location_t
xdr_dtn_bundle_payload_t = _dtnapi.xdr_dtn_bundle_payload_t
DTN_SUCCESS = _dtnapi.DTN_SUCCESS
DTN_ERRBASE = _dtnapi.DTN_ERRBASE
DTN_EINVAL = _dtnapi.DTN_EINVAL
DTN_EXDR = _dtnapi.DTN_EXDR
DTN_ECOMM = _dtnapi.DTN_ECOMM
DTN_ECONNECT = _dtnapi.DTN_ECONNECT
DTN_ETIMEOUT = _dtnapi.DTN_ETIMEOUT
DTN_ESIZE = _dtnapi.DTN_ESIZE
DTN_ENOTFOUND = _dtnapi.DTN_ENOTFOUND
DTN_EINTERNAL = _dtnapi.DTN_EINTERNAL
DTN_EINPOLL = _dtnapi.DTN_EINPOLL
DTN_EBUSY = _dtnapi.DTN_EBUSY
DTN_EVERSION = _dtnapi.DTN_EVERSION
DTN_EMSGTYPE = _dtnapi.DTN_EMSGTYPE
DTN_ENOSPACE = _dtnapi.DTN_ENOSPACE
DTN_ERRMAX = _dtnapi.DTN_ERRMAX
dtn_strerror = _dtnapi.dtn_strerror
dtn_open = _dtnapi.dtn_open
find_handle = _dtnapi.find_handle
dtn_close = _dtnapi.dtn_close
dtn_errno = _dtnapi.dtn_errno
dtn_build_local_eid = _dtnapi.dtn_build_local_eid
build_reginfo = _dtnapi.build_reginfo
dtn_register = _dtnapi.dtn_register
dtn_unregister = _dtnapi.dtn_unregister
dtn_find_registration = _dtnapi.dtn_find_registration
dtn_change_registration = _dtnapi.dtn_change_registration
dtn_bind = _dtnapi.dtn_bind
dtn_unbind = _dtnapi.dtn_unbind
class dtn_bundle_id:
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, dtn_bundle_id, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, dtn_bundle_id, name)
    __repr__ = _swig_repr
    __swig_setmethods__["source"] = _dtnapi.dtn_bundle_id_source_set
    __swig_getmethods__["source"] = _dtnapi.dtn_bundle_id_source_get
    __swig_setmethods__["creation_secs"] = _dtnapi.dtn_bundle_id_creation_secs_set
    __swig_getmethods__["creation_secs"] = _dtnapi.dtn_bundle_id_creation_secs_get
    __swig_setmethods__["creation_seqno"] = _dtnapi.dtn_bundle_id_creation_seqno_set
    __swig_getmethods__["creation_seqno"] = _dtnapi.dtn_bundle_id_creation_seqno_get
    def __init__(self, *args): 
        this = apply(_dtnapi.new_dtn_bundle_id, args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _dtnapi.delete_dtn_bundle_id
    __del__ = lambda self : None;
dtn_bundle_id_swigregister = _dtnapi.dtn_bundle_id_swigregister
dtn_bundle_id_swigregister(dtn_bundle_id)
cvar = _dtnapi.cvar

dtn_cancel = _dtnapi.dtn_cancel
class dtn_status_report:
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, dtn_status_report, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, dtn_status_report, name)
    __repr__ = _swig_repr
    __swig_setmethods__["bundle_id"] = _dtnapi.dtn_status_report_bundle_id_set
    __swig_getmethods__["bundle_id"] = _dtnapi.dtn_status_report_bundle_id_get
    __swig_setmethods__["reason"] = _dtnapi.dtn_status_report_reason_set
    __swig_getmethods__["reason"] = _dtnapi.dtn_status_report_reason_get
    __swig_setmethods__["flags"] = _dtnapi.dtn_status_report_flags_set
    __swig_getmethods__["flags"] = _dtnapi.dtn_status_report_flags_get
    __swig_setmethods__["receipt_ts_secs"] = _dtnapi.dtn_status_report_receipt_ts_secs_set
    __swig_getmethods__["receipt_ts_secs"] = _dtnapi.dtn_status_report_receipt_ts_secs_get
    __swig_setmethods__["receipt_ts_seqno"] = _dtnapi.dtn_status_report_receipt_ts_seqno_set
    __swig_getmethods__["receipt_ts_seqno"] = _dtnapi.dtn_status_report_receipt_ts_seqno_get
    __swig_setmethods__["custody_ts_secs"] = _dtnapi.dtn_status_report_custody_ts_secs_set
    __swig_getmethods__["custody_ts_secs"] = _dtnapi.dtn_status_report_custody_ts_secs_get
    __swig_setmethods__["custody_ts_seqno"] = _dtnapi.dtn_status_report_custody_ts_seqno_set
    __swig_getmethods__["custody_ts_seqno"] = _dtnapi.dtn_status_report_custody_ts_seqno_get
    __swig_setmethods__["forwarding_ts_secs"] = _dtnapi.dtn_status_report_forwarding_ts_secs_set
    __swig_getmethods__["forwarding_ts_secs"] = _dtnapi.dtn_status_report_forwarding_ts_secs_get
    __swig_setmethods__["forwarding_ts_seqno"] = _dtnapi.dtn_status_report_forwarding_ts_seqno_set
    __swig_getmethods__["forwarding_ts_seqno"] = _dtnapi.dtn_status_report_forwarding_ts_seqno_get
    __swig_setmethods__["delivery_ts_secs"] = _dtnapi.dtn_status_report_delivery_ts_secs_set
    __swig_getmethods__["delivery_ts_secs"] = _dtnapi.dtn_status_report_delivery_ts_secs_get
    __swig_setmethods__["delivery_ts_seqno"] = _dtnapi.dtn_status_report_delivery_ts_seqno_set
    __swig_getmethods__["delivery_ts_seqno"] = _dtnapi.dtn_status_report_delivery_ts_seqno_get
    __swig_setmethods__["deletion_ts_secs"] = _dtnapi.dtn_status_report_deletion_ts_secs_set
    __swig_getmethods__["deletion_ts_secs"] = _dtnapi.dtn_status_report_deletion_ts_secs_get
    __swig_setmethods__["deletion_ts_seqno"] = _dtnapi.dtn_status_report_deletion_ts_seqno_set
    __swig_getmethods__["deletion_ts_seqno"] = _dtnapi.dtn_status_report_deletion_ts_seqno_get
    __swig_setmethods__["ack_by_app_ts_secs"] = _dtnapi.dtn_status_report_ack_by_app_ts_secs_set
    __swig_getmethods__["ack_by_app_ts_secs"] = _dtnapi.dtn_status_report_ack_by_app_ts_secs_get
    __swig_setmethods__["ack_by_app_ts_seqno"] = _dtnapi.dtn_status_report_ack_by_app_ts_seqno_set
    __swig_getmethods__["ack_by_app_ts_seqno"] = _dtnapi.dtn_status_report_ack_by_app_ts_seqno_get
    def __init__(self, *args): 
        this = apply(_dtnapi.new_dtn_status_report, args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _dtnapi.delete_dtn_status_report
    __del__ = lambda self : None;
dtn_status_report_swigregister = _dtnapi.dtn_status_report_swigregister
dtn_status_report_swigregister(dtn_status_report)
dtn_send = _dtnapi.dtn_send

dtn_status_report_reason_to_str = _dtnapi.dtn_status_report_reason_to_str
class dtn_bundle:
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, dtn_bundle, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, dtn_bundle, name)
    __repr__ = _swig_repr
    __swig_setmethods__["source"] = _dtnapi.dtn_bundle_source_set
    __swig_getmethods__["source"] = _dtnapi.dtn_bundle_source_get
    __swig_setmethods__["dest"] = _dtnapi.dtn_bundle_dest_set
    __swig_getmethods__["dest"] = _dtnapi.dtn_bundle_dest_get
    __swig_setmethods__["replyto"] = _dtnapi.dtn_bundle_replyto_set
    __swig_getmethods__["replyto"] = _dtnapi.dtn_bundle_replyto_get
    __swig_setmethods__["priority"] = _dtnapi.dtn_bundle_priority_set
    __swig_getmethods__["priority"] = _dtnapi.dtn_bundle_priority_get
    __swig_setmethods__["dopts"] = _dtnapi.dtn_bundle_dopts_set
    __swig_getmethods__["dopts"] = _dtnapi.dtn_bundle_dopts_get
    __swig_setmethods__["expiration"] = _dtnapi.dtn_bundle_expiration_set
    __swig_getmethods__["expiration"] = _dtnapi.dtn_bundle_expiration_get
    __swig_setmethods__["creation_secs"] = _dtnapi.dtn_bundle_creation_secs_set
    __swig_getmethods__["creation_secs"] = _dtnapi.dtn_bundle_creation_secs_get
    __swig_setmethods__["creation_seqno"] = _dtnapi.dtn_bundle_creation_seqno_set
    __swig_getmethods__["creation_seqno"] = _dtnapi.dtn_bundle_creation_seqno_get
    __swig_setmethods__["delivery_regid"] = _dtnapi.dtn_bundle_delivery_regid_set
    __swig_getmethods__["delivery_regid"] = _dtnapi.dtn_bundle_delivery_regid_get
    __swig_setmethods__["sequence_id"] = _dtnapi.dtn_bundle_sequence_id_set
    __swig_getmethods__["sequence_id"] = _dtnapi.dtn_bundle_sequence_id_get
    __swig_setmethods__["obsoletes_id"] = _dtnapi.dtn_bundle_obsoletes_id_set
    __swig_getmethods__["obsoletes_id"] = _dtnapi.dtn_bundle_obsoletes_id_get
    __swig_setmethods__["payload"] = _dtnapi.dtn_bundle_payload_set
    __swig_getmethods__["payload"] = _dtnapi.dtn_bundle_payload_get
    __swig_setmethods__["status_report"] = _dtnapi.dtn_bundle_status_report_set
    __swig_getmethods__["status_report"] = _dtnapi.dtn_bundle_status_report_get
    def __init__(self, *args): 
        this = apply(_dtnapi.new_dtn_bundle, args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _dtnapi.delete_dtn_bundle
    __del__ = lambda self : None;
dtn_bundle_swigregister = _dtnapi.dtn_bundle_swigregister
dtn_bundle_swigregister(dtn_bundle)

dtn_recv = _dtnapi.dtn_recv
class dtn_session_info:
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, dtn_session_info, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, dtn_session_info, name)
    __repr__ = _swig_repr
    __swig_setmethods__["status"] = _dtnapi.dtn_session_info_status_set
    __swig_getmethods__["status"] = _dtnapi.dtn_session_info_status_get
    __swig_setmethods__["session"] = _dtnapi.dtn_session_info_session_set
    __swig_getmethods__["session"] = _dtnapi.dtn_session_info_session_get
    def __init__(self, *args): 
        this = apply(_dtnapi.new_dtn_session_info, args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _dtnapi.delete_dtn_session_info
    __del__ = lambda self : None;
dtn_session_info_swigregister = _dtnapi.dtn_session_info_swigregister
dtn_session_info_swigregister(dtn_session_info)

dtn_session_update = _dtnapi.dtn_session_update
dtn_poll_fd = _dtnapi.dtn_poll_fd
dtn_begin_poll = _dtnapi.dtn_begin_poll
dtn_cancel_poll = _dtnapi.dtn_cancel_poll


