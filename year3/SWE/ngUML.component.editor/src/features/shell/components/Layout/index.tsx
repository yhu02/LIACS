import React from 'react'
import { Header } from '../Header'
import './style/layout.css'

type LayoutProps = {
  children?: React.ReactNode
}

export const Layout: React.FC<LayoutProps> = ({ children }) => {
  return <>{children}</>
}
