import React from 'react'
import './_footer.css'

export const Footer: React.FC = () => {
    return (
        <footer id="Footer">
            <a href="https://liacs.leidenuniv.nl/" rel="noreferrer noopener" target="_blank" >
                <img src="/assets/liacs-logo-inverse.png" alt="Leiden Institute for Advanced Computer Science" />
            </a>
        </footer>
    )
}

export default Footer