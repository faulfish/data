// Copy from .\service\mdsp\Mdsp_intf.h

#define  MAX_RAMPON_ENTRIES     30  /* 30 entries or less                */
#define  MAX_RAMPOFF_ENTRIES    30  /* 30 entries or less                */

/* pa profile structures */
typedef int16 mdsp_pa_lut_entry_type;
typedef mdsp_pa_lut_entry_type *mdsp_pa_lut_ptr_type;
 
typedef struct
{
  uint8     step_size;                                /* duration of rampup/rampdown steps   */
  mdsp_pa_lut_entry_type  rampon[MAX_RAMPON_ENTRIES+1];   /* Ramp up entries + "end flag 0xffff" */
  mdsp_pa_lut_entry_type  rampoff[MAX_RAMPOFF_ENTRIES+1];   /* Ramp up entries + "end flag 0xffff" */
} mdsp_pa_lut_ramp_type;
 
typedef struct
{
  int8                    used_comp_val;
  mdsp_pa_lut_ramp_type     pa_ramp;
} mdsp_pa_ramp_type;