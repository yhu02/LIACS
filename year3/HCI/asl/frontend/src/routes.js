import { Navigate, useRoutes } from 'react-router-dom';
// layouts
import DashboardLayout from './layouts/dashboard';
import SimpleLayout from './layouts/simple';
import ApplicationLayout from './layouts/application'
import SplashLayout from './layouts/splash';

import SplashPage from './pages/splash/SplashPage';
import IntroPage from './pages/splash/IntroPage';
import IntroPage2 from './pages/splash/IntroPage2';
import IntroPage3 from './pages/splash/IntroPage3';
import IntroPage4 from './pages/splash/IntroPage4';

//
import BlogPage from './pages/BlogPage';
import HistoryPage from './pages/HistoryPage';
import LoginPage from './pages/LoginPage';
import Page404 from './pages/Page404';
import DictionaryPage from './pages/DictionaryPage';
import DashboardAppPage from './pages/DashboardAppPage';


import GameModePage from './pages/application/GameModePage';
import TopicPage from './pages/application/TopicPage';
import GamePage from './pages/application/GamePage';

// ----------------------------------------------------------------------

export default function Router() {
  const routes = useRoutes([
    {
      path: '',
      children: [
        {
          path: '/dashboard',
          element: <DashboardLayout />,
          children: [
            { element: <Navigate to="/dashboard/app" />, index: true },
            { path: 'app', element: <DashboardAppPage /> },
            { path: 'history', element: <HistoryPage /> },
            { path: 'dictionary', element: <DictionaryPage /> },
            { path: 'about', element: <BlogPage /> },
            { path: 'settings', element: <BlogPage /> },
          ],
        },
        {
          path: '/application',
          element: <ApplicationLayout />,
          children: [
            { element: <Navigate to="/application/topic" />, index: true },
            { path: 'gamemode', element: <GameModePage /> },
            { path: 'topic', element: <TopicPage /> },
            { path: 'score', element: <BlogPage /> },
            { path: 'game', element: <GamePage /> },
          ],
        },
        {
          path: 'login',
          element: <LoginPage />,
        },
        {
          element: <SimpleLayout />,
          children: [
            { element: <Navigate to="/splash/start" replace />, index: true },
            { path: '404', element: <Page404 /> },
            { path: '*', element: <Navigate to="/404" /> },
          ],
        },
        {
          path: '*',
          element: <Navigate to="/404" replace />,
        },
        {
          path: '/splash',
          children: [
            { element: <Navigate to="/splash/start" />, index: true },
            { path: 'start', element: <SplashPage /> },
            { path: 'intro', element: <IntroPage /> },
            { path: 'intro2', element: <IntroPage2 /> },
            { path: 'intro3', element: <IntroPage3 /> },
            { path: 'intro4', element: <IntroPage4 /> },



          ],
        },
      ]
    }
  ]);

  return routes;
}
