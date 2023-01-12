// component
import SvgColor from '../../../components/svg-color';

// ----------------------------------------------------------------------

const icon = (name) => <SvgColor src={`/assets/icons/navbar/${name}.svg`} sx={{ width: 1, height: 1 }} />;

const navConfig = [
  {
    title: 'Dashboard',
    path: '/dashboard/app',
    icon: icon('ic_analytics'),
  },
  {
    title: 'Dictionary',
    path: '/dashboard/dictionary',
    icon: icon('ic_cart'),
  },
  {
    title: 'History',
    path: '/dashboard/history',
    icon: icon('ic_user'),
  },
  {
    title: 'About',
    path: '/dashboard/about',
    icon: icon('ic_blog'),
  },
  {
    title: 'Settings',
    path: '/dashboard/settings',
    icon: icon('ic_lock'),
  },
];

export default navConfig;
