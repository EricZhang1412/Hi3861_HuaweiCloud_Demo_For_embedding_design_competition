package com.example.dibudaoying;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.drawerlayout.widget.DrawerLayout;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentTransaction;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.util.Log;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;

import com.google.android.material.bottomnavigation.BottomNavigationView;
import com.google.android.material.navigation.NavigationBarView;
import com.google.android.material.navigation.NavigationView;

public class MainActivity extends AppCompatActivity  {

    private DrawerLayout drawerLayout;
    private NavigationView navigationView;
    private BottomNavigationView bottomNavigationView;
    private HomeFragment homeFragment;
    private LibraryFragment libraryFragment;
    private ShortsFragment shortsFragment;
    private SubscriptionFragment subscriptionFragment;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        bottomNavigationView = findViewById(R.id.bottom_navigation);
        drawerLayout = findViewById(R.id.drawer_layout);
        navigationView = findViewById(R.id.nav_view);

        //
        homeFragment = new HomeFragment();
        libraryFragment = new LibraryFragment();
        shortsFragment = new ShortsFragment();
        subscriptionFragment = new SubscriptionFragment();

        Bundle bundle = new Bundle();


        getSupportFragmentManager().beginTransaction()
                .replace(R.id.fragmentContainer, homeFragment)
                .commit();

        Thread t = new Thread() {
            @Override
            public void run() {
                try {
                    HuaweiIOT hwiot=new HuaweiIOT();
                    String tempValue;
                    String humidValue;
                    String gasValue;
                    String tdsValue;
                    System.out.println("获取温度");
                    tempValue =hwiot.getdev("tempvalue");
                    humidValue=hwiot.getdev("humidvalue");
                    gasValue=hwiot.getdev("gasvalue");
                    tdsValue=hwiot.getdev("tdsvalue");
                    bundle.putString("temp",tempValue);
                    bundle.putString("humid",humidValue);
                    bundle.putString("gas",gasValue);
                    bundle.putString("tds",tdsValue);
                    Log.d("Tag", "temp: " + bundle.getString("temp"));
// 将 Bundle 对象设置给 Fragment
                    homeFragment.setArguments(bundle);
                    shortsFragment.setArguments(bundle);
                }
                catch (Exception e) {
                    e.printStackTrace();
                    System.out.println("获取失败："+e.toString());
                }
            }
        };
        t.start();
        bottomNavigationView.setOnItemSelectedListener(new NavigationBarView.OnItemSelectedListener() {
            @Override
            public boolean onNavigationItemSelected(@NonNull MenuItem item) {
                if (item.getItemId() == R.id.menu_home) {
                    showFragment(homeFragment);
                    return true;
                } else if (item.getItemId() == R.id.menu_library) {
                    showFragment(libraryFragment);
                    return true;
                } else if (item.getItemId() == R.id.menu_shorts) {
                    showFragment(shortsFragment);
                    return true;
                } else if (item.getItemId() == R.id.menu_subscription) {
                    showFragment(subscriptionFragment);
                    return true;
                }
                return false;
            }
        });


    }
    private void showFragment(Fragment fragment) {
        getSupportFragmentManager().beginTransaction()
                .replace(R.id.fragmentContainer, fragment)
                .commit();
    }

}