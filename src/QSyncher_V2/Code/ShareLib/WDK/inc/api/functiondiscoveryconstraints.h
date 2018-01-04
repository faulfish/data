///////////////////////////////////////////////////////////////////////////////
// QUERY Constraint defines
///////////////////////////////////////////////////////////////////////////////

#define MAX_FDCONSTRAINTNAME_LENGTH                     100
#define MAX_FDCONSTRAINTVALUE_LENGTH                    1000

// Common Provider specific Constraints
#define FD_QUERYCONSTRAINT_PROVIDERINSTANCEID           L"ProviderInstanceID"
#define FD_QUERYCONSTRAINT_SUBCATEGORY                  L"Subcategory"
#define FD_QUERYCONSTRAINT_RECURSESUBCATEGORY           L"RecurseSubcategory"
#define FD_QUERYCONSTRAINT_VISIBILITY                   L"Visibility"
    // FD_CONSTRAINTVALUE_VISIBILITY_DEFAULT you want just default instances (visible as defined by the provider)
    // FD_CONSTRAINTVALUE_VISIBILITY_ALL (default) you want both visible and not visible/hidden instances (as defined by the provider)
#define FD_QUERYCONSTRAINT_COMCLSCONTEXT                L"COMClsContext"

// Common Provider specific Constraints values
#define FD_CONSTRAINTVALUE_TRUE                         L"TRUE"
#define FD_CONSTRAINTVALUE_FALSE                        L"FALSE"
#define FD_CONSTRAINTVALUE_RECURSESUBCATEGORY_TRUE      FD_CONSTRAINTVALUE_TRUE
#define FD_CONSTRAINTVALUE_VISIBILITY_DEFAULT           L"0"
#define FD_CONSTRAINTVALUE_VISIBILITY_ALL               L"1"
#define FD_CONSTRAINTVALUE_COMCLSCONTEXT_INPROC_SERVER  L"1"
#define FD_CONSTRAINTVALUE_COMCLSCONTEXT_LOCAL_SERVER   L"4"

///////////////////////////////////////////////////////////////////////////////
// PNP Provider specific Constraints
#define PROVIDERPNP_QUERYCONSTRAINT_INTERFACECLASS      L"InterfaceClass"
#define PROVIDERPNP_QUERYCONSTRAINT_NOTPRESENT          L"NotPresent"
#define PROVIDERPNP_QUERYCONSTRAINT_NOTIFICATIONSONLY   L"NotifyOnly"
    // PNP_CONSTRAINTVALUE_NOTPRESENT you want "not present" instances as well
    // "FALSE" (default) you want only DIGCF_PRESENT instances.
// PNP Provider specific Constraints values
#define PNP_CONSTRAINTVALUE_NOTPRESENT                  FD_CONSTRAINTVALUE_TRUE
#define PNP_CONSTRAINTVALUE_NOTIFICATIONSONLY           FD_CONSTRAINTVALUE_TRUE

///////////////////////////////////////////////////////////////////////////////
// SSDP Provider specific Constraints
#define PROVIDERSSDP_QUERYCONSTRAINT_TYPE               L"Type"

// SSDP Provider specific Constraints values
#define SSDP_CONSTRAINTVALUE_TYPE_ALL                   L"ssdp:all"
#define SSDP_CONSTRAINTVALUE_TYPE_ROOT                  L"upnp:rootdevice"
#define SSDP_CONSTRAINTVALUE_TYPE_DEVICE_PREFIX         L"urn:schemas-upnp-org:device:"
#define SSDP_CONSTRAINTVALUE_TYPE_SVC_PREFIX            L"urn:schemas-upnp-org:service:"

#define SSDP_CONSTRAINTVALUE_TYPE_DEV_LIGHTING          SSDP_CONSTRAINTVALUE_TYPE_DEVICE_PREFIX L"Lighting:1"
#define SSDP_CONSTRAINTVALUE_TYPE_DEV_REMINDER          SSDP_CONSTRAINTVALUE_TYPE_DEVICE_PREFIX L"Reminder:1"
#define SSDP_CONSTRAINTVALUE_TYPE_DEV_POWERDEVICE       SSDP_CONSTRAINTVALUE_TYPE_DEVICE_PREFIX L"PowerDevice:1"
#define SSDP_CONSTRAINTVALUE_TYPE_DEV_IGD               SSDP_CONSTRAINTVALUE_TYPE_DEVICE_PREFIX L"InternetGatewayDevice:1"
#define SSDP_CONSTRAINTVALUE_TYPE_DEV_WANDEVICE         SSDP_CONSTRAINTVALUE_TYPE_DEVICE_PREFIX L"WANDevice:1"
#define SSDP_CONSTRAINTVALUE_TYPE_DEV_LANDEVICE         SSDP_CONSTRAINTVALUE_TYPE_DEVICE_PREFIX L"LANDevice:1"
#define SSDP_CONSTRAINTVALUE_TYPE_DEV_WANCONNDEVICE     SSDP_CONSTRAINTVALUE_TYPE_DEVICE_PREFIX L"WANConnectionDevice:1"
#define SSDP_CONSTRAINTVALUE_TYPE_DEV_LUXMETER          SSDP_CONSTRAINTVALUE_TYPE_DEVICE_PREFIX L"Luxmeter:1"
#define SSDP_CONSTRAINTVALUE_TYPE_DEV_MDARNDR           SSDP_CONSTRAINTVALUE_TYPE_DEVICE_PREFIX L"MediaRenderer:1"
#define SSDP_CONSTRAINTVALUE_TYPE_DEV_MDASRVR           SSDP_CONSTRAINTVALUE_TYPE_DEVICE_PREFIX L"MediaServer:1"

#define SSDP_CONSTRAINTVALUE_TYPE_SVC_SCANNER           SSDP_CONSTRAINTVALUE_TYPE_SVC_PREFIX L"Scanner:1"
#define SSDP_CONSTRAINTVALUE_TYPE_SVC_DIMMING           SSDP_CONSTRAINTVALUE_TYPE_SVC_PREFIX L"DimmingService:1"

///////////////////////////////////////////////////////////////////////////////
// WSD Provider specific Constraints
#define PROVIDERWSD_QUERYCONSTRAINT_DIRECTEDADDRESS     L"RemoteAddress"
#define PROVIDERWSD_QUERYCONSTRAINT_TYPE                L"Type"
#define PROVIDERWSD_QUERYCONSTRAINT_SCOPE               L"Scope"

///////////////////////////////////////////////////////////////////////////////
// NetBios Provider specific Constraints
#define PROVIDERWNET_QUERYCONSTRAINT_TYPE               L"Type"
#define PROVIDERWNET_QUERYCONSTRAINT_PROPERTIES         L"Properties"
#define PROVIDERWNET_QUERYCONSTRAINT_RESOURCETYPE       L"ResourceType"

#define WNET_CONSTRAINTVALUE_TYPE_ALL                   L"All"
#define WNET_CONSTRAINTVALUE_TYPE_SERVER                L"Server"   // Default
#define WNET_CONSTRAINTVALUE_TYPE_DOMAIN                L"Domain"

#define WNET_CONSTRAINTVALUE_PROPERTIES_ALL             L"All"
#define WNET_CONSTRAINTVALUE_PROPERTIES_LIMITED         L"Limited"  // Default

#define WNET_CONSTRAINTVALUE_RESOURCETYPE_DISK          L"Disk"             // All non-printer shares (dwDisplayType == RESOURCEDISPLAYTYPE_SHARE and dwType != RESOURCETYPE_PRINT)
#define WNET_CONSTRAINTVALUE_RESOURCETYPE_PRINTER       L"Printer"          // All printer shares (dwDisplayType == RESOURCEDISPLAYTYPE_SHARE and dwType == RESOURCETYPE_PRINT)
#define WNET_CONSTRAINTVALUE_RESOURCETYPE_DISKORPRINTER L"DiskOrPrinter"    // All shares (dwDisplayType == RESOURCEDISPLAYTYPE_SHARE)

///////////////////////////////////////////////////////////////////////////////
// PROPERTY Constraint defines
///////////////////////////////////////////////////////////////////////////////

#ifndef FDPropertyConstraint
typedef enum tagPropertyConstraint
{
    QC_EQUALS = 0,
    QC_NOTEQUAL = 1,
    QC_LESSTHAN = 2,
    QC_LESSTHANOREQUAL = 3,
    QC_GREATERTHAN = 4,
    QC_GREATERTHANOREQUAL = 5,
    QC_STARTSWITH = 6,   // Strings only
    QC_EXISTS = 7,
    QC_DOESNOTEXIST = 8,
    QC_CONTAINS = 9     // Strings and VT_VECTOR only
} PropertyConstraint;
#endif


