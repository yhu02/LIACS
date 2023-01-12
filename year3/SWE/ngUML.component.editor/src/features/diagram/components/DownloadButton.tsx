import React from 'react';
import { toPng } from 'html-to-image';
import { Button } from '@blueprintjs/core'

function downloadImage(dataUrl: any) {
  const a = document.createElement('a');

  a.setAttribute('download', 'reactflow.png');
  a.setAttribute('href', dataUrl);
  a.click();
}

export function DownloadButton() {
  const onClick = () => {
    toPng(document.querySelector('.react-flow'), {
      filter: (node: any) => {
        // we don't want to add the minimap and the controls to the image
        if (
          node?.classList?.contains('react-flow__minimap') ||
          node?.classList?.contains('react-flow__controls')
        ) {
          return false;
        }

        return true;
      },
    }).then(downloadImage);
  };

  return (
    <Button className="download-btn" onClick={onClick}>
      Download PNG  
    </Button>
  );
}

export default DownloadButton;