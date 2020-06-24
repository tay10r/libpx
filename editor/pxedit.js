var pxedit = {

  /** This function registers the callback to trigger
   * before the page gets closed.
   *
   * @param {number} callbackData A pointer to the callback data to pass
   * to the callback function.
   * @param {string} callback The name of the callback function.
   * */
  setUnloadCallback: function(callbackData, callback)
  {
    pxedit.beforeUnloadData = callbackData;
    pxedit.beforeUnloadCallback = Module.cwrap(callback, 'null', ['number']);

    window.onbeforeonload = function()
    {
      if (pxedit.beforeUnloadCallback != null) {
        pxedit.beforeUnloadCallback(pxedit.beforeUnloadData);
      }
    };
  },

  /** Initializes the application storage.
   * This will create the applications and mount the volume.
   * This function may cause an error if one of the file system
   * functions fail.
   * */
  initAppStorage: function()
  {
    FS.mkdir('/appStorage');

    FS.mount(IDBFS, {}, '/appStorage');

    FS.syncfs(true, function(err) {
      if (err == null) {
        try {
          /* Throws if file exists. */
          FS.mkdir('/appStorage/Documents');
        } catch (err) {
          /* TODO : It would be more correct
           * to only catch the exceptions due
           * to the directory already being made. */
        }
        FS.syncfs(pxedit.syncResult);
      } else {
        pxedit.syncResult(err);
      }
    });
  },

  /** Initiates a download operation.
   *
   * @param filenamePtr A pointer to the name of the file to save.
   * @param dataPtr A pointer to the file data within the WASM module.
   * @param size The size of the file, in terms of bytes.
   * */
  download: function(filenamePtr, dataPtr, size)
  {
    const anchor = document.createElement('a');
    anchor.style = 'display:none';
    document.body.appendChild(anchor);
    const view = new Uint8Array(Module.HEAPU8.buffer, dataPtr, size);
    const blob = new Blob([view], { type: 'octet/stream' });
    const url = window.URL.createObjectURL(blob);
    anchor.href = url;
    const filename = UTF8ToString(filenamePtr);
    anchor.download = filename;
    anchor.click();
    window.URL.revokeObjectURL(url);
    document.body.removeChild(anchor);
  },

  /** Sets the callback function to receive the
   * result of a sync operation.
   *
   * @param {string} callback The name of the function
   * to call within the WASM module when a sync operation completes.
   * */
  setSyncCallback: function(dataPtr, callback) {
    syncResultData = dataPtr;
    syncResultCallback = Module.cwrap(callback, 'null', ['number', 'string']);
  },

  /* Private */

  beforeUnloadData: null,
  beforeUnloadCallback: null,

  syncResultData: null,
  syncResultCallback: null,

  /** This function is called when the result of
   * a storage sync function is ready.
   *
   * @param {String} err Optionally contains an error
   * message if something went wrong. If this is null
   * then the file system operations were successful.
   * */
  syncResult: function(err) {
    if (syncResultCallback != null) {
      syncResultCallback(syncResultData, err);
    }
  }
};
