install(FILES
    wiktionary_sound.desktop
    wiktionary_sound.py
    DESTINATION ${DATA_INSTALL_DIR}/parley/plugins)

install(FILES
    mwclient/__init__.py  
    mwclient/client.py  
    mwclient/compatibility.py  
    mwclient/errors.py  
    mwclient/ex.py  
    mwclient/http.py  
    mwclient/listing.py  
    mwclient/page_nowriteapi.py  
    mwclient/page.py
    mwclient/upload.py
    mwclient/README.txt
    DESTINATION ${DATA_INSTALL_DIR}/parley/plugins/mwclient)

install(FILES
    mwclient/simplejson/__init__.py
    mwclient/simplejson/decoder.py  
    mwclient/simplejson/encoder.py    
    mwclient/simplejson/jsonfilter.py  
    mwclient/simplejson/scanner.py
    mwclient/simplejson/ordered_dict.py
    mwclient/simplejson/tool.py
    DESTINATION ${DATA_INSTALL_DIR}/parley/plugins/mwclient/simplejson)
