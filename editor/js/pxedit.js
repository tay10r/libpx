function download(filenamePtr, dataPtr, size)
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
}

window.download = download;
