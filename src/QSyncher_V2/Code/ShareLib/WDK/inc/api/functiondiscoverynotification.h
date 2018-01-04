//  CFunctionDiscoveryNotificationWrapper class.
//
//  All OS components implementing IFunctionDiscoveryNotification should derive
//  from this class rather then directly from IFunctionDiscoveryNotification
//  so that a default implementation can be provided here in case of future
//  breaking changes to the IFunctionDiscoveryNotification interface

interface IFunctionDiscoveryNotification;   // forward declaration

#ifdef __cplusplus
class CFunctionDiscoveryNotificationWrapper : public IFunctionDiscoveryNotification
{
public:
        virtual HRESULT STDMETHODCALLTYPE OnUpdate(
            /* [in] */ QueryUpdateAction enumQueryUpdateAction,
            /* [in] */ FDQUERYCONTEXT fdqcQueryContext,
            /* [in] */ __RPC__in_opt IFunctionInstance *pIFunctionInstance)
        { return S_OK; }
        virtual HRESULT STDMETHODCALLTYPE OnError(
            /* [in] */ HRESULT hr,
            /* [in] */ FDQUERYCONTEXT fdqcQueryContext,
            /* [string][in] */ __RPC__in const WCHAR *pszProvider)
        { return S_OK; }
        virtual HRESULT STDMETHODCALLTYPE OnEvent(
            /* [in] */ DWORD dwEventID,
            /* [in] */ FDQUERYCONTEXT fdqcQueryContext,
            /* [string][in] */ __RPC__in const WCHAR *pszProvider)
        { return S_OK; }
};
#endif

// Event ids
#define FD_EVENTID                  1000
#define FD_EVENTID_SEARCHCOMPLETE   FD_EVENTID
#define FD_EVENTID_ASYNCTHREADEXIT  FD_EVENTID + 1
#define FD_EVENTID_SEARCHSTART      FD_EVENTID + 2


