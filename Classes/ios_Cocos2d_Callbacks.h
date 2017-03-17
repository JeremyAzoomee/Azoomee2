
#ifdef __cplusplus
extern "C" {
#endif
    
    void navigateToBaseScene();
    void navigateToLoginScene();
    void sendMixPanelData(const char* host, const char* query);
    const char* sendGameApiRequest(const char* method, const char* responseid, const char* score);
    
#ifdef __cplusplus
}
#endif
